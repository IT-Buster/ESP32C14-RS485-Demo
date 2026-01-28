# ESP32C14-RS485-Demo
Example code for RS485 communication using ESP32

## Overview
This project demonstrates RS485 half-duplex communication using ESP32. The example includes both Master and Slave modes.

## Features
- RS485 half-duplex communication
- Automatic direction control (DE/RE pins)
- Master/Slave mode selection
- UART-based implementation
- Simple message exchange protocol

## Hardware Requirements
- ESP32 development board
- RS485 to TTL converter module (e.g., MAX485)
- Connection wires

## Pin Configuration
| Function | GPIO Pin |
|----------|----------|
| TX (A)   | GPIO 17  |
| RX (B)   | GPIO 16  |
| RTS/DE   | GPIO 4   |

## Wiring
Connect your RS485 module to ESP32:
- ESP32 GPIO17 → RS485 Module DI (Driver Input)
- ESP32 GPIO16 → RS485 Module RO (Receiver Output)
- ESP32 GPIO4 → RS485 Module DE and RE (Direction Enable, tied together)
- ESP32 GND → RS485 Module GND
- ESP32 3.3V/5V → RS485 Module VCC

For communication between two devices:
- Connect A to A and B to B of both RS485 modules
- Add 120Ω termination resistor between A and B on both ends of the bus

## Software Configuration

### Master Mode
Set in `main/rs485_example.c`:
```c
#define DEMO_MODE_MASTER    1
```

### Slave Mode
Set in `main/rs485_example.c`:
```c
#define DEMO_MODE_MASTER    0
```

## Building and Flashing

### Prerequisites
- ESP-IDF v4.4 or later installed
- ESP-IDF environment configured

### Build
```bash
idf.py build
```

### Flash
```bash
idf.py -p /dev/ttyUSB0 flash
```

### Monitor
```bash
idf.py -p /dev/ttyUSB0 monitor
```

Or all in one:
```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

## Usage

### Two Device Setup
1. Flash one ESP32 with Master mode (DEMO_MODE_MASTER = 1)
2. Flash another ESP32 with Slave mode (DEMO_MODE_MASTER = 0)
3. Connect both devices via RS485 bus
4. Power on both devices
5. Monitor serial output to see communication

### Expected Output

**Master:**
```
RS485 Master mode started
Sent: Master message #0 (bytes: 19)
Received: Slave ACK: 19 bytes received
Sent: Master message #1 (bytes: 19)
Received: Slave ACK: 19 bytes received
```

**Slave:**
```
RS485 Slave mode started
Received: Master message #0
Sent response
Received: Master message #1
Sent response
```

## Configuration Parameters
You can modify these parameters in `main/rs485_example.c`:
- `RS485_UART_NUM`: UART port number (default: UART_NUM_1)
- `RS485_TXD_PIN`: TX pin (default: GPIO17)
- `RS485_RXD_PIN`: RX pin (default: GPIO16)
- `RS485_RTS_PIN`: Direction control pin (default: GPIO4)
- `RS485_BAUD_RATE`: Communication speed (default: 9600)

## Troubleshooting

### No communication
- Check wiring connections
- Verify A and B lines are correctly connected
- Ensure both devices use the same baud rate
- Check termination resistors (120Ω on both ends)

### Garbled data
- Verify baud rate settings match
- Check for proper grounding
- Ensure cable length is not too long (max 1200m for RS485)

### Direction control issues
- Verify RTS/DE pin is correctly connected to both DE and RE pins
- Check RS485 module has proper pull-up/pull-down resistors

## License
This example code is in the Public Domain (or CC0 licensed, at your option.)

Unless required by applicable law or agreed to in writing, this
software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.
