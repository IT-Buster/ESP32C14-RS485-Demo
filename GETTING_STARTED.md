# Getting Started - Beginner's Guide

This guide will help you get started with the ESP32 RS485 Demo project, even if you're new to ESP32 development.

## What You'll Need

### Hardware
1. **2x ESP32 Development Boards** ($5-10 each)
   - Any ESP32 variant works (ESP32, ESP32-S2, ESP32-C3, etc.)
   - Recommended: ESP32 DevKit V1 or NodeMCU-32S

2. **2x MAX485 RS485 Modules** ($1-2 each)
   - Look for modules with labels: DI, RO, DE, RE, A, B
   - 3.3V/5V compatible versions recommended

3. **2x 120Ω Resistors** (< $1)
   - 1/4W or 1/2W rating
   - Used for bus termination

4. **Jumper Wires** ($2-5 for a set)
   - Male-to-Female wires
   - At least 20cm length

5. **Breadboards** (optional, $3-5 each)
   - For easier prototyping

6. **2x USB Cables** 
   - Micro-USB or USB-C depending on your ESP32 board

### Software
1. **ESP-IDF** (Free)
   - Espressif's official development framework
   - Download from: https://docs.espressif.com/projects/esp-idf/

2. **USB Drivers** (if needed)
   - CP210x or CH340 drivers
   - Automatically installed on most systems

3. **Terminal Program** (Free)
   - Linux/Mac: Built-in (screen or minicom)
   - Windows: PuTTY or Tera Term

## Step-by-Step Setup

### Step 1: Install ESP-IDF

#### Windows
```bash
# Download ESP-IDF installer from:
# https://dl.espressif.com/dl/esp-idf/
# Run the installer and follow the wizard
```

#### Linux/Mac
```bash
# Install prerequisites
sudo apt-get install git wget flex bison gperf python3 python3-pip python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

# Clone ESP-IDF
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git

# Install ESP-IDF
cd ~/esp/esp-idf
./install.sh esp32

# Set up environment (add to ~/.bashrc for permanent)
. ~/esp/esp-idf/export.sh
```

### Step 2: Get the RS485 Demo Code

```bash
# Clone this repository
git clone https://github.com/IT-Buster/ESP32C14-RS485-Demo.git
cd ESP32C14-RS485-Demo
```

### Step 3: Configure the Project

#### Option A: Use Master Mode (Device 1)
No changes needed! The default is Master mode.

#### Option B: Configure Slave Mode (Device 2)
Edit `main/rs485_example.c`:
```c
// Change this line:
#define DEMO_MODE_MASTER    1

// To this:
#define DEMO_MODE_MASTER    0
```

### Step 4: Build the Project

```bash
# Make sure ESP-IDF environment is active
# (You should see (esp-idf) in your terminal prompt)

# Build the project
idf.py build
```

Expected output:
```
Project build complete.
```

### Step 5: Connect ESP32 to Computer

1. Connect ESP32 to computer via USB cable
2. Note the port name:
   - Linux: `/dev/ttyUSB0` or `/dev/ttyACM0`
   - Mac: `/dev/cu.usbserial-*`
   - Windows: `COM3`, `COM4`, etc.

### Step 6: Flash the Firmware

```bash
# Replace /dev/ttyUSB0 with your port
idf.py -p /dev/ttyUSB0 flash
```

Expected output:
```
Hash of data verified.
Leaving...
Hard resetting via RTS pin...
```

### Step 7: Wire the Hardware

#### Master Device (ESP32 #1)
```
ESP32 Pin    →    MAX485 Module
---------         -------------
GPIO 17      →    DI
GPIO 16      →    RO
GPIO 4       →    DE and RE (connect together)
GND          →    GND
5V           →    VCC
```

#### Slave Device (ESP32 #2)
```
ESP32 Pin    →    MAX485 Module
---------         -------------
GPIO 17      →    DI
GPIO 16      →    RO
GPIO 4       →    DE and RE (connect together)
GND          →    GND
5V           →    VCC
```

#### Connect RS485 Bus
```
Master MAX485    →    Slave MAX485
-------------         -------------
A                →    A
B                →    B
```

#### Add Termination Resistors
```
Master Side: 120Ω resistor between A and B
Slave Side:  120Ω resistor between A and B
```

### Step 8: Monitor the Output

On both devices, run:
```bash
idf.py -p /dev/ttyUSB0 monitor
```

Press `Ctrl+]` to exit the monitor.

## Expected Results

### Master Device Output
```
I (320) RS485_DEMO: RS485 Demo Application Starting...
I (330) RS485_DEMO: RS485 initialized on UART1
I (350) RS485_DEMO: Starting in MASTER mode
I (360) RS485_DEMO: RS485 Master mode started
I (370) RS485_DEMO: Sent: Master message #0
I (2380) RS485_DEMO: Received: Slave ACK: 19 bytes received
I (2390) RS485_DEMO: Sent: Master message #1
I (4400) RS485_DEMO: Received: Slave ACK: 19 bytes received
```

### Slave Device Output
```
I (320) RS485_DEMO: RS485 Demo Application Starting...
I (330) RS485_DEMO: RS485 initialized on UART1
I (350) RS485_DEMO: Starting in SLAVE mode
I (360) RS485_DEMO: RS485 Slave mode started
I (2370) RS485_DEMO: Received: Master message #0
I (2380) RS485_DEMO: Sent response
I (4380) RS485_DEMO: Received: Master message #1
I (4390) RS485_DEMO: Sent response
```

## Common Problems and Solutions

### Problem: "idf.py: command not found"
**Solution**: ESP-IDF environment not set up. Run:
```bash
. ~/esp/esp-idf/export.sh
```

### Problem: "Permission denied" on flash
**Solution**: Add user to dialout group (Linux):
```bash
sudo usermod -a -G dialout $USER
# Log out and log back in
```

### Problem: No communication between devices
**Solutions**:
1. Check A and B wiring (don't cross them!)
2. Verify termination resistors are installed
3. Check that one device is Master, other is Slave
4. Ensure both devices are powered on
5. Verify GND is connected between modules

### Problem: Build errors
**Solution**: Clean and rebuild:
```bash
idf.py fullclean
idf.py build
```

### Problem: Can't find USB port
**Solution**: 
- Linux: `ls /dev/ttyUSB*` or `ls /dev/ttyACM*`
- Mac: `ls /dev/cu.*`
- Windows: Check Device Manager → Ports

## Next Steps

1. **Experiment**: Try changing the baud rate or GPIO pins
2. **Add More Slaves**: Connect additional slave devices
3. **Advanced Example**: Try the Modbus-like protocol in `examples/advanced_modbus/`
4. **Build Your Project**: Use this as a starting point for your own application

## Learning Resources

- ESP-IDF Documentation: https://docs.espressif.com/projects/esp-idf/
- RS485 Standard: https://en.wikipedia.org/wiki/RS-485
- MAX485 Datasheet: Search for "MAX485 datasheet"

## Getting Help

1. Read the README.md file
2. Check QUICK_REFERENCE.md for commands
3. Review WIRING.md for connection help
4. Open an issue on GitHub

## Success!

If you see both devices communicating, congratulations! You've successfully set up RS485 communication between two ESP32 devices.

You can now:
- Modify the code for your needs
- Add more features
- Build industrial control systems
- Create sensor networks
- Experiment with Modbus protocols

Happy coding! 🚀
