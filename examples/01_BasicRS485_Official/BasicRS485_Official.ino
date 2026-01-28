//Author: eletechsup
//More information welcome to : http://www.485io.com 
//Arduino IDE 2.2.1
//ES32C14 Expansion Board for ESP32 38PIN BOARD
//RS485 receiving and sending test

/*
 * Hardware Connection:
 * ES32C14 has built-in RS485 transceiver connected to UART0
 * 
 * GPIO1 (TX0) - Connected internally to RS485 TX
 * GPIO3 (RX0) - Connected internally to RS485 RX
 * GPIO22      - DE/RE pin (Direction Enable/Receive Enable)
 * 
 * RS485 Terminal Block:
 * A+ - RS485 Data+ line
 * B- - RS485 Data- line
 * GND - Common ground
 * 
 * Operation:
 * - Serial uses UART0 (hardware RS485 interface)
 * - GPIO22 LOW  = Receive mode (DE/RE disabled)
 * - GPIO22 HIGH = Transmit mode (DE/RE enabled)
 * - Baudrate: 115200 bps
 * 
 * Note: Serial Monitor will NOT work because UART0 is used for RS485!
 * Use external RS485 device or another ES32C14 board for testing.
 */

String receivedData;
const int RS485RD = 22;  // DE/RE control pin (Direction Enable/Receive Enable)

void setup() {                
  Serial.begin(115200);      // UART0 at 115200 baud for RS485 communication
  Serial.setTimeout(5);      // 5ms timeout for readString()
  pinMode(RS485RD, OUTPUT);
  digitalWrite(RS485RD, LOW); // Start in receive mode
}

void loop() {  
  if (Serial.available()) {
      receivedData = Serial.readString();
      digitalWrite(RS485RD, HIGH);  // Switch to transmit mode
      Serial.print("receivedData: ");
      Serial.println(receivedData);
      delay(100);
      digitalWrite(RS485RD, LOW);   // Switch back to receive mode
    }
}
