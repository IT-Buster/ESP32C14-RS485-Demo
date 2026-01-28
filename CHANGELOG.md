# Changelog

All notable changes to this project will be documented in this file.

## [1.0.0] - 2026-01-28

### Added
- Initial RS485 demo implementation for ESP32
- Basic RS485 communication with Master/Slave modes
- Half-duplex communication support
- Automatic direction control using RTS pin
- ESP-IDF project structure with CMakeLists
- Comprehensive README with setup instructions
- Detailed wiring guide (WIRING.md)
- Quick reference guide (QUICK_REFERENCE.md)
- Hardware schematic documentation (SCHEMATIC.md)
- Advanced example with Modbus-like protocol
- CRC-16 error checking in advanced example
- Device addressing support in advanced example
- .gitignore for ESP-IDF projects

### Features
- UART-based RS485 implementation
- Configurable baud rate (default: 9600)
- Configurable GPIO pins
- Simple message exchange protocol
- Master sends periodic messages
- Slave responds with acknowledgments
- Example logging and debugging

### Documentation
- Complete setup and usage guide
- Pin configuration tables
- Troubleshooting section
- Expected output examples
- Multi-device setup instructions
- Cable specifications
- Component list

### Examples
- Basic Master/Slave example (main/rs485_example.c)
- Advanced protocol example (examples/advanced_modbus/)
