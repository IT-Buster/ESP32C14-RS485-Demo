# ES32C14 Pinout Reference

## RS485 Hardware Connection

The ES32C14 board has **built-in RS485 transceiver** connected to **UART0**.

### Pins:
- **GPIO1 (TX)** - RS485 TX (connected internally to RS485 transceiver)
- **GPIO3 (RX)** - RS485 RX (connected internally to RS485 transceiver)
- **GPIO22** - DE/RE (Direction Enable/Receive Enable)

### Terminal Block (Right side of board):
```
A+  - RS485 A+ (Data+)
B-  - RS485 B- (Data-)
GND - Ground
```

### Important Notes:

1. **Serial (UART0) is used for RS485** - Serial Monitor will NOT work!
   - You cannot use Serial.print() for debugging to USB
   - Serial is hardwired to RS485 interface

2. Use **WiFi** or **Serial2** (GPIO16/17) for debugging
   - Example: `Serial2.begin(115200, SERIAL_8N1, 16, 17);`
   - Then use `Serial2.print()` for debug output

3. **GPIO22 Control:**
   - **LOW** = Receive mode (normal state)
   - **HIGH** = Transmit mode (only when sending)

4. **Half-Duplex Operation:**
   - Cannot send and receive simultaneously
   - Must switch between TX and RX modes using GPIO22

## LED Indicator
- **GPIO2** - Built-in LED (can be used for diagnostics)
  - Use LED blink patterns to indicate status
  - Example: 2 blinks = success, 5 blinks = error

## Additional GPIO Pins (for Serial2 debugging)

If you need debugging output while using RS485:

```
GPIO16 - Serial2 RX (can be used for debug console)
GPIO17 - Serial2 TX (can be used for debug console)
```

Example setup:
```cpp
Serial2.begin(115200, SERIAL_8N1, 16, 17);
Serial2.println("Debug message");
```

## Power Pins

- **5V** - 5V power input (from USB or external)
- **3.3V** - 3.3V regulated output
- **GND** - Ground

## Complete Pin Reference

| GPIO | Function | RS485 Usage | Alternative Use |
|------|----------|-------------|----------------|
| 1 | TX0 | RS485 TX | (Reserved) |
| 3 | RX0 | RS485 RX | (Reserved) |
| 22 | I/O | DE/RE Control | General I/O |
| 2 | I/O | LED (optional) | General I/O |
| 16 | RX2 | - | Debug UART RX |
| 17 | TX2 | - | Debug UART TX |

## Wiring Diagram (ASCII)

```
ES32C14 Board
┌─────────────────────────────────┐
│                                 │
│  ┌──────────────────┐          │
│  │   ESP32 Module   │          │
│  │                  │          │
│  │  GPIO1 (TX) ─────┼──┐       │
│  │  GPIO3 (RX) ─────┼──┼──┐    │
│  │  GPIO22     ─────┼──┼──┼──┐ │
│  └──────────────────┘  │  │  │ │
│                        │  │  │ │
│  ┌──────────────────┐  │  │  │ │
│  │ RS485 Transceiver│  │  │  │ │
│  │                  │  │  │  │ │
│  │  TXD ◄───────────┼──┘  │  │ │
│  │  RXD ◄───────────┼─────┘  │ │
│  │  DE/RE ◄─────────┼────────┘ │
│  │                  │          │
│  │  A ──────────────┼─→ A+     │
│  │  B ──────────────┼─→ B-     │
│  │  GND ────────────┼─→ GND    │
│  └──────────────────┘          │
│                                 │
│         Terminal Block          │
│         [A+] [B-] [GND]         │
└─────────────────────────────────┘
```

## Typical Connections

### For HT73 Temperature/Humidity Sensor:
```
ES32C14 Terminal    →    HT73 Sensor
─────────────────────────────────────
A+                  →    A+ (yellow)
B-                  →    B- (white)
GND                 →    GND (black)
External PSU        →    VCC (red, 5-24V)
```

### For Testing with Another ES32C14:
```
ES32C14 #1          →    ES32C14 #2
─────────────────────────────────────
A+                  →    A+
B-                  →    B-
GND                 →    GND
```

## Troubleshooting Pin Issues

### Issue: No communication
**Check:**
- GPIO22 is connected and working (controls DE/RE)
- A+ and B- are connected correctly (not swapped)
- GND is connected between all devices

### Issue: Cannot debug
**Solution:**
- Use Serial2 on GPIO16/17 for debug output
- Or use WiFi web interface for monitoring
- Or use LED blink patterns on GPIO2

### Issue: Board not responding
**Check:**
- Power supply is adequate (5V via USB or external)
- ESP32 module is properly seated
- No short circuits on RS485 lines
