# RS485 Quick Reference Guide

## Common Commands

### Build Project
```bash
idf.py build
```

### Flash to ESP32
```bash
idf.py -p /dev/ttyUSB0 flash
```

### Monitor Serial Output
```bash
idf.py -p /dev/ttyUSB0 monitor
```

### Clean Build
```bash
idf.py fullclean
```

## Configuration Quick Reference

### Change to Master Mode
In `main/rs485_example.c`, set:
```c
#define DEMO_MODE_MASTER    1
```

### Change to Slave Mode
In `main/rs485_example.c`, set:
```c
#define DEMO_MODE_MASTER    0
```

### Change GPIO Pins
In `main/rs485_example.c`, modify:
```c
#define RS485_TXD_PIN       17  // Change TX pin
#define RS485_RXD_PIN       16  // Change RX pin
#define RS485_RTS_PIN       4   // Change RTS/DE pin
```

### Change Baud Rate
In `main/rs485_example.c`, modify:
```c
#define RS485_BAUD_RATE     9600  // Change to desired baud rate
```

## Pin Reference for Popular ESP32 Boards

### ESP32 DevKit V1
- TX: GPIO 17
- RX: GPIO 16
- RTS/DE: GPIO 4

### ESP32-WROOM-32
- TX: GPIO 17
- RX: GPIO 16
- RTS/DE: GPIO 4

### ESP32-C3
- TX: GPIO 6
- RX: GPIO 7
- RTS/DE: GPIO 10

## RS485 Module Connection

### MAX485 Module
```
ESP32        MAX485
------       ------
GPIO17   →   DI
GPIO16   →   RO
GPIO4    →   DE & RE (tied together)
GND      →   GND
5V       →   VCC
```

### Terminal Block
```
A (non-inverting)
B (inverting)
```

## Testing Procedure

1. **Single Device Test**
   - Flash device in master mode
   - Monitor output
   - Should show "No response from slave" (expected without slave)

2. **Two Device Test**
   - Flash Device 1 as Master (DEMO_MODE_MASTER = 1)
   - Flash Device 2 as Slave (DEMO_MODE_MASTER = 0)
   - Connect A to A, B to B
   - Add 120Ω resistors on both ends
   - Power on and monitor both

3. **Expected Communication**
   - Master sends message every 2 seconds
   - Slave responds with acknowledgment
   - Both devices log messages

## Troubleshooting Quick Fixes

| Problem | Solution |
|---------|----------|
| No output on monitor | Check USB port, try different cable |
| Compile error | Verify ESP-IDF installed and sourced |
| Upload failed | Check port permissions, try `sudo` |
| No RS485 communication | Verify A/B wiring, check termination |
| Garbled data | Match baud rates on both devices |
| Build error | Run `idf.py fullclean` then rebuild |

## Terminal Commands for Linux Port Permissions

```bash
# Add user to dialout group (one time)
sudo usermod -a -G dialout $USER

# Or use sudo for flash
sudo idf.py -p /dev/ttyUSB0 flash monitor
```

## Typical Serial Monitor Output

### Master Output
```
I (320) RS485_DEMO: RS485 Demo Application Starting...
I (330) RS485_DEMO: RS485 initialized on UART1
I (340) RS485_DEMO: TX: GPIO17, RX: GPIO16, RTS/DE: GPIO4
I (350) RS485_DEMO: Starting in MASTER mode
I (360) RS485_DEMO: RS485 Master mode started
I (370) RS485_DEMO: Sent: Master message #0
I (2380) RS485_DEMO: Received: Slave ACK: 19 bytes received
```

### Slave Output
```
I (320) RS485_DEMO: RS485 Demo Application Starting...
I (330) RS485_DEMO: RS485 initialized on UART1
I (340) RS485_DEMO: TX: GPIO17, RX: GPIO16, RTS/DE: GPIO4
I (350) RS485_DEMO: Starting in SLAVE mode
I (360) RS485_DEMO: RS485 Slave mode started
I (2370) RS485_DEMO: Received: Master message #0
I (2380) RS485_DEMO: Sent response
```

## Exit Monitor
Press `Ctrl+]` to exit the serial monitor.
