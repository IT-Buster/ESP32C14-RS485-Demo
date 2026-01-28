/*
 * ES32C14 Modbus RTU Master - HT73 Sensor Reader
 * 
 * Reads temperature and humidity from HT73 sensor via Modbus RTU
 * Slave ID: 5
 * Function Code: 03 (Read Holding Registers)
 * Registers: 0-1 (Humidity at reg 0, Temperature at reg 1)
 * 
 * Hardware Connection:
 * ES32C14 has built-in RS485 transceiver connected to UART0
 * 
 * GPIO1 (TX0) - Connected internally to RS485 TX
 * GPIO3 (RX0) - Connected internally to RS485 RX
 * GPIO22      - DE/RE pin (Direction Enable/Receive Enable)
 * GPIO2       - Built-in LED (used for status indication)
 * 
 * RS485 Terminal Block:
 * A+ - Connect to HT73 A+
 * B- - Connect to HT73 B-
 * GND - Connect to HT73 GND
 * 
 * HT73 Sensor Wiring:
 * HT73 A+  → ES32C14 A+
 * HT73 B-  → ES32C14 B-
 * HT73 GND → ES32C14 GND
 * HT73 VCC → Power supply (5-24V depending on HT73 model)
 * 
 * Required Library:
 * Install "ModbusMaster" by Doc Walker (4-20ma)
 * Arduino IDE: Tools → Manage Libraries → Search "ModbusMaster"
 * 
 * IMPORTANT NOTE:
 * Since Serial (UART0) is used for RS485 communication, you CANNOT
 * use Serial Monitor for debugging. Options for debugging:
 * 1. Use WiFi and send debug info to web server or UDP
 * 2. Use Serial2 on different GPIO pins (e.g., GPIO16/17)
 * 3. Use LED blink patterns (implemented below)
 * 4. Use external UART logger on Serial2
 * 
 * LED Blink Patterns:
 * - 2 quick blinks = Successful read
 * - 5 fast blinks = Communication error
 * 
 * Author: Demo for ES32C14 board
 * Based on ModbusMaster library examples
 */

#include <ModbusMaster.h>

const int RS485_DE_RE = 22;
const int LED_PIN = 2;

ModbusMaster node;

void preTransmission() {
  digitalWrite(RS485_DE_RE, HIGH);  // Transmit mode
  digitalWrite(LED_PIN, HIGH);
}

void postTransmission() {
  digitalWrite(RS485_DE_RE, LOW);   // Receive mode
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  pinMode(RS485_DE_RE, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RS485_DE_RE, LOW);
  
  // Serial for RS485 (UART0)
  // 9600 baud, 8 data bits, No parity, 1 stop bit
  Serial.begin(9600, SERIAL_8N1);
  
  // Modbus slave ID 5 (typical for HT73 sensors)
  node.begin(5, Serial);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  
  delay(1000);
}

void loop() {
  // Read 2 holding registers starting at address 0
  // Register 0: Humidity (value * 10)
  // Register 1: Temperature (value * 10)
  uint8_t result = node.readHoldingRegisters(0, 2);
  
  if (result == node.ku8MBSuccess) {
    uint16_t humidity = node.getResponseBuffer(0);
    uint16_t temperature = node.getResponseBuffer(1);
    
    // Values are multiplied by 10 in HT73
    // For example: 235 means 23.5°C, 651 means 65.1% RH
    float humidityReal = humidity / 10.0;
    float temperatureReal = temperature / 10.0;
    
    // DEBUGGING OPTIONS:
    // Option 1: Use WiFi to send data to server/UDP
    // Option 2: Use Serial2 for debug output (uncomment below)
    
    // Serial2.begin(115200, SERIAL_8N1, 16, 17);  // RX=16, TX=17
    // Serial2.print("Humidity: ");
    // Serial2.print(humidityReal);
    // Serial2.print("% RH, Temperature: ");
    // Serial2.print(temperatureReal);
    // Serial2.println("°C");
    
    // LED blink pattern: success (2 quick blinks)
    blinkLED(2, 100);
  } else {
    // Communication error
    // Possible error codes:
    // 0xE0 - Invalid response
    // 0xE1 - Invalid CRC
    // 0xE2 - Invalid exception code
    // 0xE3 - Invalid function code
    // 0xE4 - Response timeout
    
    // LED blink pattern: error (5 fast blinks)
    blinkLED(5, 50);
  }
  
  delay(5000);  // Read every 5 seconds
}

void blinkLED(int times, int delayMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayMs);
    digitalWrite(LED_PIN, LOW);
    delay(delayMs);
  }
}
