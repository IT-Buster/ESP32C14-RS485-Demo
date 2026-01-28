# Hardware Specifications

## ES32C14 Expansion Board

### Overview
The ES32C14 is an expansion board designed for ESP32 38-pin development modules. It features a built-in RS485 transceiver, making it ideal for industrial communication applications.

### Board Features
- **Built-in RS485 transceiver** (connected to UART0)
- **Screw terminal block** for easy RS485 wiring
- **DE/RE control** via GPIO22
- **Compact design** for DIN rail mounting
- **LED indicators** for power and status

### Technical Specifications

#### RS485 Interface
- **Transceiver Chip:** Built-in (likely MAX485 or equivalent)
- **Data Rate:** Up to 250 kbps (tested at 9600-115200 bps)
- **Bus Length:** Up to 1200 meters (per RS485 standard)
- **Number of Nodes:** Up to 32 devices (standard RS485)
- **Termination:** External 120Ω resistors required for long cables

#### Electrical Specifications
- **Operating Voltage:** 5V (via USB or external)
- **Logic Level:** 3.3V (ESP32 native)
- **RS485 Voltage:** Differential ±5V
- **Power Consumption:** ~200mA typical (ESP32 + RS485)

#### GPIO Connections
| Function | GPIO Pin | Description |
|----------|----------|-------------|
| RS485 TX | GPIO1 (TX0) | UART0 transmit (hardwired) |
| RS485 RX | GPIO3 (RX0) | UART0 receive (hardwired) |
| DE/RE Control | GPIO22 | Direction Enable/Receive Enable |
| LED | GPIO2 | Built-in LED (optional use) |
| Debug TX | GPIO17 | Serial2 TX (available) |
| Debug RX | GPIO16 | Serial2 RX (available) |

#### Terminal Block Pinout
```
┌─────────────────────┐
│  A+   B-   GND      │ ← RS485 Terminal Block
└─────────────────────┘
   │    │    │
   │    │    └── Ground reference
   │    └──────── RS485 Data- (B)
   └───────────── RS485 Data+ (A)
```

### Physical Dimensions
- **Length:** ~60mm (typical)
- **Width:** ~40mm (typical)
- **Height:** ~20mm (with ESP32 module)
- **Weight:** ~15g (without ESP32 module)

### Environmental Specifications
- **Operating Temperature:** 0°C to 60°C
- **Storage Temperature:** -20°C to 80°C
- **Humidity:** 10% to 90% non-condensing

### Mounting
- Compatible with DIN rail mounting (check specific model)
- Screw holes for panel mounting (check specific model)

## ESP32 Module Requirements

### Compatible ESP32 Modules
The ES32C14 works with standard 38-pin ESP32 development modules:
- ESP32-WROOM-32
- ESP32-WROOM-32U (with external antenna)
- ESP32-WROVER series

### ESP32 Module Specifications
- **CPU:** Dual-core Xtensa LX6 @ 240 MHz
- **RAM:** 520 KB SRAM
- **Flash:** 4 MB (typical)
- **WiFi:** 802.11 b/g/n
- **Bluetooth:** BLE 4.2
- **GPIO:** 34 pins (some reserved for flash/RS485)

## RS485 Transceiver Specifications

### Typical Transceiver Characteristics
Based on MAX485 or equivalent:
- **Driver Output:** ±1.5V to ±5V differential
- **Receiver Input:** ±200mV differential sensitivity
- **Common Mode Range:** -7V to +12V
- **Data Rate:** 2.5 Mbps maximum (typically used at 9600-115200)
- **Driver/Receiver Enable:** Controlled via GPIO22

### Protection Features
- **ESD Protection:** Typically ±15kV (check specific model)
- **Thermal Shutdown:** Built-in (if using MAX485)
- **Short Circuit Protection:** Check specific model datasheet

## Power Supply Requirements

### Power Options
1. **USB Power (5V)**
   - Standard micro-USB or USB-C
   - Minimum 500mA capacity
   - Easiest for development

2. **External 5V Power**
   - Via dedicated power input (check board)
   - Minimum 1A recommended for reliability
   - Required for remote installations

### Power Consumption Breakdown
| Component | Typical Current | Peak Current |
|-----------|----------------|--------------|
| ESP32 (WiFi active) | 80-150mA | 240mA |
| ESP32 (WiFi idle) | 15-30mA | - |
| RS485 Transceiver | 1-5mA | 60mA |
| **Total (Active)** | **100-200mA** | **300mA** |

### Power Supply Recommendations
- Use regulated 5V supply
- Minimum 500mA capacity
- Use 1A or higher for safety margin
- Add 100µF capacitor near ESP32 for stability

## Communication Specifications

### UART Configuration
- **UART Port:** UART0 (hardware RS485)
- **Baud Rates:** 4800, 9600, 19200, 38400, 57600, 115200
- **Data Bits:** 8 (standard)
- **Parity:** None (N), Even (E), Odd (O)
- **Stop Bits:** 1 or 2
- **Flow Control:** RTS via GPIO22 (DE/RE)

### Common Modbus RTU Settings
```
Baud Rate: 9600
Data Bits: 8
Parity: None
Stop Bits: 1
Notation: 9600 8N1
```

### RS485 Bus Characteristics
- **Topology:** Multi-drop bus (not star)
- **Cable Type:** Twisted pair (recommended)
- **Characteristic Impedance:** 120Ω
- **Termination:** 120Ω resistors at both ends
- **Maximum Stub Length:** 6 meters (keep short)

## Recommended Accessories

### Essential Items
1. **120Ω Resistors** (1/4W or 1/2W)
   - Quantity: 2 (for termination)
   - Install at both ends of RS485 bus

2. **Twisted Pair Cable**
   - Cat5/Cat6 Ethernet cable works well
   - Or dedicated RS485 cable
   - Length: As needed (up to 1200m)

3. **Screwdriver**
   - Small flat-head for terminal blocks
   - Size: 2.5mm typical

### Optional Items
1. **USB-Serial Adapter** (for debugging)
   - Connect to GPIO16/17 for Serial2 output
   - Alternative debugging method

2. **USB-RS485 Adapter** (for testing)
   - Useful for testing sensors independently
   - Helps identify wiring issues

3. **Logic Analyzer** (advanced troubleshooting)
   - For protocol analysis
   - Debugging communication issues

4. **Multimeter**
   - Check voltages
   - Verify continuity
   - Measure resistance

## Sensor Compatibility

### Tested Sensors
- **HT73** - Temperature/Humidity (tested, works)
- Other Modbus RTU sensors should work

### Modbus RTU Device Requirements
- **Protocol:** Modbus RTU (not Modbus ASCII or TCP)
- **Interface:** RS485 (not RS232 or TTL)
- **Baud Rate:** Configurable, recommend 9600
- **Slave ID:** 1-247 (configurable on sensor)
- **Function Codes:** Commonly FC03 (holding) or FC04 (input)

## Certifications and Compliance

### Safety
- Check board for CE/FCC markings
- Designed for low-voltage applications (<50V)
- Not intrinsically safe (not for hazardous areas)

### EMC Considerations
- RS485 differential signaling provides noise immunity
- Use shielded cable in noisy environments
- Keep RS485 cables away from power lines
- Add common mode choke if needed

## Ordering Information

### Where to Buy
- **Official Distributor:** http://www.485io.com (eletechsup)
- Search for: "ES32C14 Expansion Board ESP32"
- Alternative: "ESP32 RS485 board"

### Package Contents (Typical)
- ES32C14 expansion board
- May or may not include ESP32 module (check listing)
- Documentation may be limited

### What You Need to Purchase Separately
- ESP32 module (if not included)
- USB cable (Micro-USB or USB-C)
- RS485 cable/wire
- 120Ω termination resistors
- Sensors or RS485 devices to communicate with

## Version Information

### Board Revisions
The ES32C14 may have different hardware revisions. Check your board for:
- Version number (if marked)
- Component changes between versions
- Manufacturer information

### Identifying Your Board
Look for markings:
- "ES32C14" silkscreen
- Version number (v1.0, v2.0, etc.)
- Manufacturer: eletechsup or other

## Maintenance and Care

### Handling
- Handle by edges to avoid damaging components
- Avoid touching exposed circuitry
- Use anti-static precautions when handling

### Cleaning
- Power off before cleaning
- Use isopropyl alcohol for flux removal
- Avoid water or conductive cleaning agents

### Storage
- Store in anti-static bag
- Keep in dry environment
- Avoid extreme temperatures

### Troubleshooting Hardware
- Inspect for physical damage
- Check for loose connections
- Verify ESP32 module is seated properly
- Look for solder bridge or cold joints

## Technical Support

### Resources
- **Manufacturer:** http://www.485io.com (eletechsup)
- **This Repository:** https://github.com/IT-Buster/ESP32C14-RS485-Demo
- **ESP32 Docs:** https://docs.espressif.com/

### Common Issues
See [TROUBLESHOOTING.md](../docs/TROUBLESHOOTING.md) for detailed solutions.

## Legal and Safety Notices

### Disclaimer
- Use at your own risk
- Not certified for safety-critical applications
- Verify specifications before use in production

### Warranty
- Check with seller for warranty terms
- Typically 30-90 days limited warranty

### Safety Warnings
- Do not exceed voltage ratings
- Do not use in explosive atmospheres
- Ensure proper grounding and isolation
- Follow local electrical codes

## Revision History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2024 | Initial release |

---

*For latest information, check manufacturer website: http://www.485io.com*
