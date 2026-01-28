# Troubleshooting Guide

## Common Issues and Solutions

### 1. No Modbus Communication (Error Messages)

**Symptoms:**
- Error code 226 or 0xE2 (timeout)
- `modbusConnected: false`
- Sensor works with USB-RS485 adapter but not with ESP32
- Scanner finds no devices

**Solutions:**

#### Solution 1: Swap A+ and B- Wires (MOST COMMON!)
This fixes 80% of connection issues!
```
Try swapping:
A+ → B-
B- → A+
```
Why: Different manufacturers use different polarity standards.

#### Solution 2: Check GND Connection
- Verify GND is connected between ES32C14 and sensor
- Use multimeter to verify continuity
- Check for loose wires

#### Solution 3: Verify Sensor Power Supply
- Check voltage requirement (5V, 12V, or 24V)
- Measure actual voltage at sensor terminals
- Ensure power supply has enough current capacity

#### Solution 4: Try Different Baudrate
```cpp
// Instead of:
Serial.begin(9600, SERIAL_8N1);

// Try:
Serial.begin(4800, SERIAL_8N1);   // Slower, more reliable
// or
Serial.begin(19200, SERIAL_8N1);  // Faster
```

#### Solution 5: Try Different Slave ID
Common Slave IDs to test:
- 1 (default for many devices)
- 5 (common for HT73)
- 247 (broadcast address - some devices respond)

#### Solution 6: Add Termination Resistor
For cable length > 3 meters:
```
Add 120Ω resistor between A+ and B-
On both ES32C14 and sensor
```

### 2. ESP32 Crashes During Scan

**Symptoms:**
- Browser shows "Connection interrupted"
- ESP32 restarts unexpectedly
- Uptime resets to 0
- WiFi disconnects

**Causes:**
- Watchdog timer timeout
- Memory issues
- Blocking operations too long

**Solutions:**

#### Solution 1: Reduce Scan Range
```cpp
// Instead of scanning 1-247:
for (uint8_t id = 1; id <= 10; id++)
```

#### Solution 2: Add Yield Calls
```cpp
for (uint8_t id = 1; id <= 247; id++) {
  // ... scan code ...
  yield();  // Allow other tasks to run
  delay(200);  // Give system time to recover
}
```

#### Solution 3: Increase Timeout
In ModbusMaster library configuration:
```cpp
// Increase response timeout
node.setTimeout(2000);  // 2 seconds instead of default
```

#### Solution 4: Use Single Device Test Instead
Instead of full scan, test one device at a time:
```cpp
void testSingleDevice(uint8_t slaveId) {
  node.begin(slaveId, Serial);
  uint8_t result = node.readHoldingRegisters(0, 2);
  // ... check result ...
}
```

### 3. Cannot Debug with Serial Monitor

**Cause:**
Serial (UART0) is used for RS485 communication, not USB debugging.

**Solutions:**

#### Solution 1: Use WiFi Web Interface
```cpp
#include <WiFi.h>
#include <WebServer.h>

WiFi.softAP("ES32C14-Debug", "12345678");
WebServer server(80);

server.on("/", []() {
  String html = "Debug info: " + debugVariable;
  server.send(200, "text/html", html);
});

server.begin();
```

#### Solution 2: Use Serial2 for Debug Output
```cpp
void setup() {
  // RS485 on Serial (UART0)
  Serial.begin(9600, SERIAL_8N1);
  
  // Debug on Serial2 (UART2)
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX=16, TX=17
}

void loop() {
  // Use Serial2 for debugging
  Serial2.println("Debug message");
  Serial2.print("Value: ");
  Serial2.println(someVariable);
}
```

Connect USB-Serial adapter to GPIO16/17 for debug output.

#### Solution 3: Use LED Blink Patterns
```cpp
void setup() {
  pinMode(2, OUTPUT);
}

void indicateError(int errorCode) {
  // Blink error code
  for (int i = 0; i < errorCode; i++) {
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(200);
  }
  delay(1000);
}

// Usage:
if (result == node.ku8MBSuccess) {
  blinkLED(2, 100);  // Success: 2 quick blinks
} else {
  blinkLED(5, 50);   // Error: 5 fast blinks
}
```

#### Solution 4: Use External UART Logger
Connect logic analyzer or UART logger to Serial2 pins.

### 4. Intermittent Communication

**Symptoms:**
- Works sometimes, fails other times
- Random timeout errors
- Garbled data

**Solutions:**

#### Solution 1: Add Termination Resistors
```
Add 120Ω resistor between A+ and B-:
- On ES32C14 side
- On sensor side
```

#### Solution 2: Use Shielded Cable
- Replace plain wire with twisted pair
- Cat5/Cat6 Ethernet cable works well
- Ground shield at ONE end only

#### Solution 3: Keep Cables Short
For testing:
- Use cables < 10 meters
- Avoid running parallel to power cables
- Keep away from motors and high-voltage equipment

#### Solution 4: Check for Loose Connections
- Verify all screw terminals are tight
- Check for broken wires
- Use multimeter to test continuity

#### Solution 5: Reduce Baud Rate
```cpp
// More reliable for noisy environments:
Serial.begin(4800, SERIAL_8N1);
```

### 5. Sketch Upload Issues

**Symptoms:**
- "Failed to connect to ESP32"
- Upload timeout
- Cannot enter bootloader

**Solutions:**

#### Solution 1: Hold BOOT Button
1. Hold down BOOT button
2. Press and release RESET button
3. Release BOOT button
4. Try upload again

#### Solution 2: Check USB Cable
- Use data cable, not charge-only cable
- Try different USB port
- Try different cable

#### Solution 3: Check Driver
- Install CP210x or CH340 driver
- Check Device Manager (Windows)
- Verify port appears in Arduino IDE

#### Solution 4: Reduce Upload Speed
In Arduino IDE:
- Tools → Upload Speed → 115200 (instead of 921600)

### 6. WiFi Not Working (Scanner Example)

**Symptoms:**
- Cannot connect to "ES32C14-Scanner"
- Cannot access web interface
- WiFi AP not visible

**Solutions:**

#### Solution 1: Check WiFi Antenna
- Ensure antenna is connected (if external)
- Check for physical damage

#### Solution 2: Verify Code
```cpp
WiFi.softAP("ES32C14-Scanner", "12345678");
delay(1000);  // Wait for AP to start

IPAddress IP = WiFi.softAPIP();
Serial2.print("AP IP address: ");
Serial2.println(IP);  // Should be 192.168.4.1
```

#### Solution 3: Check Network Settings
- Disable other WiFi networks on your device
- Connect manually to "ES32C14-Scanner"
- Password: "12345678"
- Open browser to: http://192.168.4.1

#### Solution 4: Restart ESP32
- Press RESET button
- Wait 5 seconds
- Try connecting again

### 7. ModbusMaster Library Issues

**Symptoms:**
- Compilation errors
- "ModbusMaster.h: No such file or directory"
- Undefined references

**Solutions:**

#### Solution 1: Install Library
1. Arduino IDE → Tools → Manage Libraries
2. Search "ModbusMaster"
3. Install "ModbusMaster by Doc Walker"
4. Version 2.0.1 or later

#### Solution 2: Verify Installation
Check that library appears in:
- Arduino IDE → Sketch → Include Library → ModbusMaster

#### Solution 3: Manual Installation
1. Download from: https://github.com/4-20ma/ModbusMaster
2. Extract to Arduino/libraries/ folder
3. Restart Arduino IDE

### 8. Wrong Data Values

**Symptoms:**
- Temperature shows 230°C instead of 23°C
- Humidity shows 650% instead of 65%
- Values seem multiplied by 10

**Solution:**
HT73 sensor returns values multiplied by 10.

```cpp
// WRONG:
float temp = node.getResponseBuffer(1);  // Returns 235

// CORRECT:
float temp = node.getResponseBuffer(1) / 10.0;  // Returns 23.5
```

### 9. Modbus Error Codes

Common error codes from ModbusMaster library:

| Code | Meaning | Solution |
|------|---------|----------|
| 0xE0 | Invalid response | Check wiring, try swap A+/B- |
| 0xE1 | Invalid CRC | Check cable quality, add termination |
| 0xE2 | Invalid exception | Check Slave ID and function code |
| 0xE3 | Invalid function | Sensor doesn't support this function |
| 0xE4 | Response timeout | Increase timeout, check connection |

## Hardware Verification Tests

### Test 1: LED Blink Test (No RS485 needed)
```cpp
void setup() {
  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}
```
Expected: LED blinks every second
If fails: Hardware issue, check ESP32 module

### Test 2: WiFi AP Test (No RS485 needed)
Upload Scanner example and check WiFi:
- "ES32C14-Scanner" network should appear
- Connect and access http://192.168.4.1
- Should see web interface

### Test 3: Loopback Test
Short A+ to B- on terminal block:
- Upload Echo example
- Should receive its own transmitted messages
- LED should blink every 2 seconds

### Test 4: External Tool Test
Use USB-RS485 adapter with modpoll:
```bash
# Test sensor directly with modpoll
modpoll -m rtu -b 9600 -p none -a 5 -r 0 -c 2 /dev/ttyUSB0

# Should read registers 0-1 from Slave ID 5
```

## Diagnostic Checklist

Before asking for help, verify:

- [ ] A+ and B- connected correctly (try both ways)
- [ ] GND connected between all devices
- [ ] Power supply correct voltage for sensor
- [ ] Sensor is powered on (check with multimeter)
- [ ] Correct Slave ID in code (try 1, 5, 247)
- [ ] Correct baudrate (9600 most common)
- [ ] Termination resistors if cable > 3m
- [ ] Cable length within limits
- [ ] ModbusMaster library installed
- [ ] Correct board selected in Arduino IDE
- [ ] Upload successful without errors
- [ ] Tried swapping A+ and B- wires

## Getting More Help

### Collect This Information:

1. **Hardware:**
   - ES32C14 board version
   - Sensor model and version
   - Cable type and length
   - Power supply voltage

2. **Software:**
   - Arduino IDE version
   - ESP32 board package version
   - ModbusMaster library version
   - Which example you're using

3. **Error Details:**
   - Exact error message
   - LED blink pattern observed
   - Error code number
   - What you've already tried

### Where to Ask:

1. GitHub Issues: https://github.com/IT-Buster/ESP32C14-RS485-Demo/issues
2. Arduino Forum: https://forum.arduino.cc/
3. ESP32 Forum: https://www.esp32.com/

### Useful Debug Commands:

```cpp
// Add to your code for troubleshooting:
Serial2.begin(115200, SERIAL_8N1, 16, 17);

Serial2.print("Modbus result: 0x");
Serial2.println(result, HEX);

Serial2.print("Response buffer size: ");
Serial2.println(node.getResponseBufferLength());

Serial2.print("Last error: 0x");
Serial2.println(node.ku8MBInvalidCRC, HEX);
```

## Success Indicators

You know it's working when:

✓ LED blinks at regular intervals (Echo example)
✓ Scanner finds at least one device (ID 5 typically)
✓ 2 quick blinks after read (Master example)
✓ Web interface shows scan results
✓ Temperature/humidity values are reasonable (10-40°C, 20-90% RH)

## Still Not Working?

If you've tried everything:

1. Test sensor with USB-RS485 adapter first
   - Confirms sensor works
   - Identifies correct Slave ID
   - Verifies baudrate

2. Test ES32C14 loopback (short A+ to B-)
   - Confirms RS485 transceiver works
   - Verifies GPIO22 control works

3. Simplify setup
   - Use shortest possible cables
   - Test with one device only
   - Use lowest baudrate (4800)

4. Try different sensor
   - Borrow known-working device
   - Rules out sensor issues

Good luck! Most issues are simple wiring problems (reversed A+/B-).
