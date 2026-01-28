/*
 * ES32C14 Modbus Scanner with WiFi Web Interface
 * 
 * Scans for Modbus RTU devices on the RS485 bus
 * Default scan range: Slave IDs 1-10 (can be extended to 1-247)
 * Tests both Function Code 03 (Holding Registers) and FC04 (Input Registers)
 * 
 * Features:
 * - WiFi Access Point mode for easy access
 * - Web interface to start scans and view results
 * - No need for Serial Monitor (uses WiFi instead)
 * - Scan results displayed in browser
 * 
 * Hardware Connection:
 * ES32C14 has built-in RS485 transceiver connected to UART0
 * 
 * GPIO1 (TX0) - Connected internally to RS485 TX
 * GPIO3 (RX0) - Connected internally to RS485 RX
 * GPIO22      - DE/RE pin (Direction Enable/Receive Enable)
 * 
 * RS485 Terminal Block:
 * A+ - Connect to slave devices A+
 * B- - Connect to slave devices B-
 * GND - Connect to slave devices GND
 * 
 * Required Libraries:
 * 1. ModbusMaster by Doc Walker (4-20ma)
 * 2. WiFi (built-in with ESP32 board package)
 * 3. WebServer (built-in with ESP32 board package)
 * 
 * Usage:
 * 1. Upload sketch to ES32C14
 * 2. Connect to WiFi network: "ES32C14-Scanner" (password: "12345678")
 * 3. Open browser and go to: http://192.168.4.1
 * 4. Click "Start Scan" button
 * 5. Results will be displayed in browser
 * 
 * Extending Scan Range:
 * To scan full Modbus address range (1-247), change in handleScan():
 *   for (uint8_t id = 1; id <= 247; id++)
 * Warning: Full scan takes several minutes!
 * 
 * Author: Demo for ES32C14 board
 */

#include <ModbusMaster.h>
#include <WiFi.h>
#include <WebServer.h>

const int RS485_DE_RE = 22;
const char* AP_SSID = "ES32C14-Scanner";
const char* AP_PASS = "12345678";

ModbusMaster node;
WebServer server(80);
String scanResults = "";

void preTransmission() {
  digitalWrite(RS485_DE_RE, HIGH);
}

void postTransmission() {
  digitalWrite(RS485_DE_RE, LOW);
}

void setup() {
  pinMode(RS485_DE_RE, OUTPUT);
  digitalWrite(RS485_DE_RE, LOW);
  
  // Serial for RS485 (UART0)
  Serial.begin(9600, SERIAL_8N1);
  
  // Initialize Modbus
  node.begin(1, Serial);  // Will change dynamically during scan
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  
  // Start WiFi Access Point
  WiFi.softAP(AP_SSID, AP_PASS);
  
  // Wait for AP to start
  delay(100);
  
  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/scan", handleScan);
  server.begin();
  
  scanResults = "Ready to scan. Click 'Start Scan' button.\n";
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ES32C14 Modbus Scanner</title>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 20px; background-color: #f0f0f0; }";
  html += "h1 { color: #333; }";
  html += "button { background-color: #4CAF50; color: white; padding: 15px 32px; ";
  html += "text-align: center; font-size: 16px; margin: 10px 0; cursor: pointer; ";
  html += "border: none; border-radius: 4px; }";
  html += "button:hover { background-color: #45a049; }";
  html += "pre { background-color: #fff; padding: 15px; border: 1px solid #ddd; ";
  html += "border-radius: 4px; overflow-x: auto; }";
  html += ".info { background-color: #e7f3fe; border-left: 6px solid #2196F3; ";
  html += "padding: 10px; margin: 10px 0; }";
  html += "</style></head><body>";
  html += "<h1>ES32C14 Modbus RTU Scanner</h1>";
  html += "<div class='info'>";
  html += "<strong>WiFi AP:</strong> " + String(AP_SSID) + "<br>";
  html += "<strong>IP Address:</strong> " + WiFi.softAPIP().toString() + "<br>";
  html += "<strong>Scan Range:</strong> Slave IDs 1-10<br>";
  html += "<strong>Functions:</strong> FC03 (Holding Registers), FC04 (Input Registers)";
  html += "</div>";
  html += "<button onclick=\"location.href='/scan'\">Start Scan (1-10)</button>";
  html += "<h2>Scan Results:</h2>";
  html += "<pre>" + scanResults + "</pre>";
  html += "<p><em>Page auto-refreshes after scan. Or <a href='/'>click here to refresh</a>.</em></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleScan() {
  scanResults = "Scanning Slave IDs 1-10...\n";
  scanResults += "Testing FC03 (Read Holding Registers) and FC04 (Read Input Registers)\n";
  scanResults += "================================================================================\n\n";
  
  int devicesFound = 0;
  
  for (uint8_t id = 1; id <= 10; id++) {
    scanResults += "Slave " + String(id) + ": ";
    node.begin(id, Serial);
    
    bool foundFC03 = false;
    bool foundFC04 = false;
    
    // Test FC03 (Read Holding Registers)
    uint8_t result = node.readHoldingRegisters(0, 2);
    if (result == node.ku8MBSuccess) {
      scanResults += "FC03 OK ";
      foundFC03 = true;
    }
    
    delay(100);  // Small delay between tests
    
    // Test FC04 (Read Input Registers)
    result = node.readInputRegisters(0, 2);
    if (result == node.ku8MBSuccess) {
      scanResults += "FC04 OK";
      foundFC04 = true;
    }
    
    if (!foundFC03 && !foundFC04) {
      scanResults += "No response";
    } else {
      devicesFound++;
    }
    
    scanResults += "\n";
    delay(200);  // Delay between devices
  }
  
  scanResults += "\n================================================================================\n";
  scanResults += "Scan complete! Devices found: " + String(devicesFound) + "\n";
  scanResults += "\nNotes:\n";
  scanResults += "- If no devices found, check A+/B- wiring (try swapping)\n";
  scanResults += "- Verify all devices are powered on\n";
  scanResults += "- Check GND connection between ES32C14 and slaves\n";
  scanResults += "- Verify baudrate is 9600 on all devices\n";
  
  // Redirect back to main page to show results
  server.sendHeader("Location", "/");
  server.send(303);
}
