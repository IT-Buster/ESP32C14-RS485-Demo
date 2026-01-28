/*
 * ES32C14 RS485 Echo Test
 * Based on eletechsup example with enhancements
 * 
 * Features:
 * - LED indicator on GPIO2 (blinks when transmitting)
 * - Auto-send test message every 2 seconds
 * - Echoes received data back to sender
 * - Support for testing with external RS485 device
 * 
 * Hardware Connection:
 * ES32C14 has built-in RS485 transceiver connected to UART0
 * 
 * GPIO1 (TX0) - Connected internally to RS485 TX
 * GPIO3 (RX0) - Connected internally to RS485 RX
 * GPIO22      - DE/RE pin (Direction Enable/Receive Enable)
 * GPIO2       - Built-in LED (used for TX indicator)
 * 
 * RS485 Terminal Block:
 * A+ - RS485 Data+ line
 * B- - RS485 Data- line
 * GND - Common ground
 * 
 * Testing:
 * 1. Connect another ES32C14 or RS485 device
 * 2. Connect A+ to A+, B- to B-, GND to GND
 * 3. LED will blink every 2 seconds when sending
 * 4. Received data is echoed back with "Echo: " prefix
 * 
 * Author: Based on eletechsup example (http://www.485io.com)
 * Enhanced with LED indicator and auto-test functionality
 */

const int RS485_DE_RE = 22;  // Direction Enable/Receive Enable
const int LED_PIN = 2;        // Built-in LED

void setup() {
  Serial.begin(9600);  // Standard Modbus baudrate
  pinMode(RS485_DE_RE, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RS485_DE_RE, LOW);  // Start in receive mode
  digitalWrite(LED_PIN, LOW);
  
  delay(1000);
  sendMessage("ES32C14 RS485 Ready!\n");
}

void loop() {
  // Check for incoming data
  if (Serial.available()) {
    String received = "";
    while (Serial.available()) {
      char c = Serial.read();
      received += c;
      delay(10);  // Small delay to allow buffer to fill (RS485 timing)
    }
    
    // Echo back
    sendMessage("Echo: " + received);
  }
  
  // Send test message every 2 seconds
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 2000) {
    sendMessage("Test message " + String(millis()/1000) + "s\n");
    lastSend = millis();
  }
}

void sendMessage(String msg) {
  digitalWrite(LED_PIN, HIGH);        // Turn on LED
  digitalWrite(RS485_DE_RE, HIGH);    // Transmit mode
  delay(10);                          // Wait for mode switch (RS485 transceiver settling time)
  
  Serial.print(msg);
  Serial.flush();                     // Wait for transmission to complete
  
  delay(10);                          // Wait before switching back (ensure transmission complete)
  digitalWrite(RS485_DE_RE, LOW);     // Receive mode
  digitalWrite(LED_PIN, LOW);         // Turn off LED
}
