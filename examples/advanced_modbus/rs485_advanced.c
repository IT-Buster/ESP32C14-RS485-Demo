/*
 * Advanced RS485 Example with Simple Modbus-like Protocol
 * 
 * This example demonstrates a more advanced RS485 communication
 * with addressing, CRC checking, and command/response protocol
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "RS485_ADV";

// RS485 Configuration
#define RS485_UART_NUM      UART_NUM_1
#define RS485_TXD_PIN       17
#define RS485_RXD_PIN       16
#define RS485_RTS_PIN       4

#define RS485_BAUD_RATE     9600
#define RS485_BUF_SIZE      256

// Protocol definitions
#define MASTER_ADDR         0x01
#define SLAVE_ADDR          0x02
#define BROADCAST_ADDR      0xFF

// Command codes
#define CMD_READ_DATA       0x03
#define CMD_WRITE_DATA      0x06
#define CMD_PING           0x08

// Packet structure
typedef struct {
    uint8_t addr;           // Device address
    uint8_t cmd;            // Command code
    uint8_t len;            // Data length
    uint8_t data[64];       // Data payload
    uint16_t crc;           // CRC checksum
} rs485_packet_t;

// Simple CRC-16 calculation
static uint16_t calc_crc16(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xFFFF;
    
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    
    return crc;
}

// Pack packet into byte array
static int pack_packet(const rs485_packet_t *pkt, uint8_t *buf)
{
    int idx = 0;
    buf[idx++] = pkt->addr;
    buf[idx++] = pkt->cmd;
    buf[idx++] = pkt->len;
    
    for (int i = 0; i < pkt->len; i++) {
        buf[idx++] = pkt->data[i];
    }
    
    // Calculate CRC over address, command, length, and data
    uint16_t crc = calc_crc16(buf, idx);
    buf[idx++] = crc & 0xFF;
    buf[idx++] = (crc >> 8) & 0xFF;
    
    return idx;
}

// Unpack byte array into packet
static bool unpack_packet(const uint8_t *buf, int len, rs485_packet_t *pkt)
{
    if (len < 5) return false; // Minimum packet size
    
    int idx = 0;
    pkt->addr = buf[idx++];
    pkt->cmd = buf[idx++];
    pkt->len = buf[idx++];
    
    if (pkt->len > 64 || len < (3 + pkt->len + 2)) return false;
    
    for (int i = 0; i < pkt->len; i++) {
        pkt->data[i] = buf[idx++];
    }
    
    // Extract CRC
    uint16_t recv_crc = buf[idx] | (buf[idx + 1] << 8);
    
    // Calculate expected CRC
    uint16_t calc_crc = calc_crc16(buf, idx);
    
    if (recv_crc != calc_crc) {
        ESP_LOGW(TAG, "CRC mismatch: recv=0x%04X, calc=0x%04X", recv_crc, calc_crc);
        return false;
    }
    
    return true;
}

static void rs485_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = RS485_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    ESP_ERROR_CHECK(uart_param_config(RS485_UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(RS485_UART_NUM, RS485_TXD_PIN, RS485_RXD_PIN,
                                  RS485_RTS_PIN, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(RS485_UART_NUM, RS485_BUF_SIZE * 2, 
                                        0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_set_mode(RS485_UART_NUM, UART_MODE_RS485_HALF_DUPLEX));

    ESP_LOGI(TAG, "Advanced RS485 initialized");
}

// Send packet
static void send_packet(const rs485_packet_t *pkt)
{
    uint8_t buf[128];
    int len = pack_packet(pkt, buf);
    uart_write_bytes(RS485_UART_NUM, buf, len);
    ESP_LOGI(TAG, "Sent: addr=0x%02X cmd=0x%02X len=%d", pkt->addr, pkt->cmd, pkt->len);
}

// Receive packet with timeout
static bool receive_packet(rs485_packet_t *pkt, int timeout_ms)
{
    uint8_t buf[128];
    int len = uart_read_bytes(RS485_UART_NUM, buf, sizeof(buf), pdMS_TO_TICKS(timeout_ms));
    
    if (len > 0) {
        if (unpack_packet(buf, len, pkt)) {
            ESP_LOGI(TAG, "Recv: addr=0x%02X cmd=0x%02X len=%d", pkt->addr, pkt->cmd, pkt->len);
            return true;
        }
    }
    
    return false;
}

// Master task
static void master_task(void *arg)
{
    rs485_packet_t tx_pkt, rx_pkt;
    int counter = 0;

    ESP_LOGI(TAG, "Master started (addr=0x%02X)", MASTER_ADDR);

    while (1) {
        // Send PING command
        tx_pkt.addr = SLAVE_ADDR;
        tx_pkt.cmd = CMD_PING;
        tx_pkt.len = 2;
        tx_pkt.data[0] = counter >> 8;
        tx_pkt.data[1] = counter & 0xFF;
        
        send_packet(&tx_pkt);
        counter++;
        
        // Wait for response
        if (receive_packet(&rx_pkt, 1000)) {
            if (rx_pkt.addr == MASTER_ADDR && rx_pkt.cmd == CMD_PING) {
                ESP_LOGI(TAG, "PING response received from slave");
            }
        } else {
            ESP_LOGW(TAG, "No response from slave");
        }
        
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

// Slave task
static void slave_task(void *arg)
{
    rs485_packet_t rx_pkt, tx_pkt;

    ESP_LOGI(TAG, "Slave started (addr=0x%02X)", SLAVE_ADDR);

    while (1) {
        if (receive_packet(&rx_pkt, 100)) {
            // Check if packet is for us or broadcast
            if (rx_pkt.addr == SLAVE_ADDR || rx_pkt.addr == BROADCAST_ADDR) {
                // Process command
                switch (rx_pkt.cmd) {
                    case CMD_PING:
                        // Send PING response
                        tx_pkt.addr = MASTER_ADDR;
                        tx_pkt.cmd = CMD_PING;
                        tx_pkt.len = rx_pkt.len;
                        memcpy(tx_pkt.data, rx_pkt.data, rx_pkt.len);
                        vTaskDelay(pdMS_TO_TICKS(10)); // Small delay
                        send_packet(&tx_pkt);
                        ESP_LOGI(TAG, "PING response sent");
                        break;
                        
                    default:
                        ESP_LOGW(TAG, "Unknown command: 0x%02X", rx_pkt.cmd);
                        break;
                }
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Advanced RS485 Demo Starting...");
    
    rs485_init();

    // Change to 0 for slave mode, 1 for master mode
    #define IS_MASTER 1
    
    #if IS_MASTER
    xTaskCreate(master_task, "master", 4096, NULL, 10, NULL);
    #else
    xTaskCreate(slave_task, "slave", 4096, NULL, 10, NULL);
    #endif
}
