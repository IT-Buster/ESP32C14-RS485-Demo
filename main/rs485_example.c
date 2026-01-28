/*
 * RS485 Communication Demo for ESP32
 * 
 * This example demonstrates RS485 communication using ESP32 UART
 * Features:
 * - Master/Slave mode selection
 * - Half-duplex communication
 * - Automatic direction control
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "RS485_DEMO";

// RS485 Configuration
#define RS485_UART_NUM      UART_NUM_1
#define RS485_TXD_PIN       17
#define RS485_RXD_PIN       16
#define RS485_RTS_PIN       4  // Direction control pin (DE/RE)

#define RS485_BAUD_RATE     9600
#define RS485_BUF_SIZE      1024

// Demo mode: 0 = Slave, 1 = Master
#define DEMO_MODE_MASTER    1

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

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(RS485_UART_NUM, &uart_config));

    // Set UART pins (TX, RX, RTS, CTS)
    ESP_ERROR_CHECK(uart_set_pin(RS485_UART_NUM, 
                                  RS485_TXD_PIN, 
                                  RS485_RXD_PIN,
                                  RS485_RTS_PIN,
                                  UART_PIN_NO_CHANGE));

    // Install UART driver using an event queue
    ESP_ERROR_CHECK(uart_driver_install(RS485_UART_NUM, 
                                        RS485_BUF_SIZE * 2, 
                                        0, 
                                        0, 
                                        NULL, 
                                        0));

    // Set RS485 half duplex mode
    ESP_ERROR_CHECK(uart_set_mode(RS485_UART_NUM, UART_MODE_RS485_HALF_DUPLEX));

    ESP_LOGI(TAG, "RS485 initialized on UART%d", RS485_UART_NUM);
    ESP_LOGI(TAG, "TX: GPIO%d, RX: GPIO%d, RTS/DE: GPIO%d", 
             RS485_TXD_PIN, RS485_RXD_PIN, RS485_RTS_PIN);
}

static void rs485_master_task(void *arg)
{
    uint8_t data[128];
    int counter = 0;

    ESP_LOGI(TAG, "RS485 Master mode started");

    while (1) {
        // Prepare message
        int len = snprintf((char *)data, sizeof(data), 
                          "Master message #%d\r\n", counter++);

        // Send data
        int txBytes = uart_write_bytes(RS485_UART_NUM, data, len);
        ESP_LOGI(TAG, "Sent: %s (bytes: %d)", data, txBytes);

        // Wait for response
        vTaskDelay(pdMS_TO_TICKS(100));

        // Read response if available
        int length = 0;
        ESP_ERROR_CHECK(uart_get_buffered_data_len(RS485_UART_NUM, (size_t*)&length));
        
        if (length > 0) {
            int len = uart_read_bytes(RS485_UART_NUM, data, length, pdMS_TO_TICKS(100));
            if (len > 0) {
                data[len] = 0; // Null-terminate
                ESP_LOGI(TAG, "Received: %s", data);
            }
        }

        // Send message every 2 seconds
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

static void rs485_slave_task(void *arg)
{
    uint8_t data[128];

    ESP_LOGI(TAG, "RS485 Slave mode started");

    while (1) {
        // Wait for data
        int length = 0;
        ESP_ERROR_CHECK(uart_get_buffered_data_len(RS485_UART_NUM, (size_t*)&length));

        if (length > 0) {
            int len = uart_read_bytes(RS485_UART_NUM, data, length, pdMS_TO_TICKS(100));
            if (len > 0) {
                data[len] = 0; // Null-terminate
                ESP_LOGI(TAG, "Received: %s", data);

                // Send echo response
                int response_len = snprintf((char *)data, sizeof(data), 
                                           "Slave ACK: %d bytes received\r\n", len);
                uart_write_bytes(RS485_UART_NUM, data, response_len);
                ESP_LOGI(TAG, "Sent response");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "RS485 Demo Application Starting...");
    
    // Initialize RS485
    rs485_init();

#if DEMO_MODE_MASTER
    ESP_LOGI(TAG, "Starting in MASTER mode");
    xTaskCreate(rs485_master_task, "rs485_master", 4096, NULL, 10, NULL);
#else
    ESP_LOGI(TAG, "Starting in SLAVE mode");
    xTaskCreate(rs485_slave_task, "rs485_slave", 4096, NULL, 10, NULL);
#endif

    ESP_LOGI(TAG, "RS485 Demo started successfully");
}
