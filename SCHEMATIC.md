# Hardware Schematic

## Basic Connection Diagram

```
                        RS485 Bus
                   (Twisted Pair Cable)
                            |
        120Ω                |                120Ω
    +---/\/\/\---+----------+----------+---/\/\/\---+
    |            |                     |            |
    A            B                     A            B
    |            |                     |            |
+---+------------+---+             +---+------------+---+
|                    |             |                    |
|  MAX485 Module     |             |  MAX485 Module     |
|  (Master Side)     |             |  (Slave Side)      |
|                    |             |                    |
|  VCC  GND DI RO DE |             |  VCC  GND DI RO DE |
+--+----+---+--+--+--+             +--+----+---+--+--+--+
   |    |   |  |  |                   |    |   |  |  |
   |    |   |  |  +---+               |    |   |  |  +---+
   |    |   |  |      |               |    |   |  |      |
   |    |   |  |      |               |    |   |  |      |
   5V  GND  |  |     RE               5V  GND  |  |     RE
           TX RX    (tied to DE)             TX RX    (tied to DE)
            |  |                              |  |
            |  |                              |  |
        +---+--+----+                     +---+--+----+
        |            |                    |            |
        |   ESP32    |                    |   ESP32    |
        |   Master   |                    |   Slave    |
        |            |                    |            |
        | GPIO17(TX) |                    | GPIO17(TX) |
        | GPIO16(RX) |                    | GPIO16(RX) |
        | GPIO4(RTS) |                    | GPIO4(RTS) |
        |            |                    |            |
        +------------+                    +------------+
```

## Pin Connections Detail

### Master ESP32 to MAX485
```
ESP32 Pin    Function    MAX485 Pin    Description
---------    --------    ----------    -----------
GPIO 17      TX          DI            Transmit Data In
GPIO 16      RX          RO            Receive Data Out
GPIO 4       RTS         DE            Driver Enable
GPIO 4       RTS         RE            Receiver Enable
GND          Ground      GND           Common Ground
5V/3.3V      Power       VCC           Power Supply
```

### Slave ESP32 to MAX485
```
ESP32 Pin    Function    MAX485 Pin    Description
---------    --------    ----------    -----------
GPIO 17      TX          DI            Transmit Data In
GPIO 16      RX          RO            Receive Data Out
GPIO 4       RTS         DE            Driver Enable
GPIO 4       RTS         RE            Receiver Enable
GND          Ground      GND           Common Ground
5V/3.3V      Power       VCC           Power Supply
```

### RS485 Bus Connections
```
Master MAX485    Signal    Slave MAX485
-------------    ------    ------------
A                A line    A
B                B line    B
```

## Component List

### Required Components
1. 2x ESP32 Development Board (any variant)
2. 2x MAX485 or similar RS485 transceiver module
3. 2x 120Ω resistors (1/4W)
4. Twisted pair cable (for RS485 bus)
5. Breadboard and jumper wires (for prototyping)
6. 2x USB cables (for programming and power)

### Optional Components
- Terminal blocks for permanent connections
- DIN rail mounting for industrial installations
- Surge protection for outdoor use
- Common mode choke for EMI suppression

## RS485 Module Variants

### MAX485 Module
- Most common and cheap
- Requires external 120Ω termination
- 3.3V and 5V compatible

### MAX3485 Module
- Enhanced version of MAX485
- Better noise immunity
- Same pinout

### SP485 Module
- Industrial grade
- Wide voltage range
- Better ESD protection

## Power Considerations

### Power Supply Options
1. **USB Power**: Simplest for testing
   - Each ESP32 powered via USB
   - Separate grounds may need connection

2. **Common Supply**: Best for production
   - Single power supply for all devices
   - Common ground reference
   - Better noise immunity

3. **Isolated Power**: For long distances
   - Separate isolated supplies
   - Use isolated RS485 transceivers
   - No common ground needed

## Cable Specifications

### Recommended Cable Types
1. **CAT5/CAT5e/CAT6**: Good for most applications
   - Use one twisted pair for A/B
   - Up to 100m distance
   - Readily available

2. **Industrial RS485 Cable**: For harsh environments
   - Shielded twisted pair
   - Up to 1200m distance
   - Better noise immunity

### Cable Installation Tips
- Keep cable away from power lines
- Avoid sharp bends
- Use shielded cable in noisy environments
- Ground shield at one end only

## Testing Points

```
Measurement Point    Expected Value    Notes
-----------------    --------------    -----
VCC to GND           5V or 3.3V        Power supply
A to B (idle)        ~0V               Differential voltage
A to GND             ~2.5V             Common mode voltage
B to GND             ~2.5V             Common mode voltage
A to B (active)      ±1.5V to ±6V      During transmission
```

## Safety Notes

1. **ESD Protection**: Handle modules carefully
2. **Polarity**: Don't reverse A and B lines
3. **Voltage**: Use correct supply voltage (3.3V or 5V)
4. **Grounding**: Proper grounding prevents damage
5. **Hot Plugging**: Avoid connecting/disconnecting while powered
