# Advanced RS485 Example

This example demonstrates a more sophisticated RS485 communication protocol with:

- Device addressing
- CRC-16 error checking
- Command/Response protocol
- Packet structure similar to Modbus

## Protocol Format

Each packet consists of:
- Address (1 byte)
- Command (1 byte)
- Data Length (1 byte)
- Data (0-64 bytes)
- CRC-16 (2 bytes)

## Commands

- `CMD_READ_DATA` (0x03): Read data from device
- `CMD_WRITE_DATA` (0x06): Write data to device
- `CMD_PING` (0x08): Ping/echo test

## Usage

To use this advanced example instead of the simple one:

1. Copy `rs485_advanced.c` to `main/` directory
2. Update `main/CMakeLists.txt` to use `rs485_advanced.c` instead of `rs485_example.c`
3. Build and flash

Or create a separate project using this as the main source file.
