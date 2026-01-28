# RS485 Wiring Guide

## Basic Single Connection (2 devices)

### Device 1 (Master) - ESP32 to RS485 Module
```
ESP32 Pin    →    RS485 Module
---------         -------------
GPIO 17      →    DI (Data In)
GPIO 16      →    RO (Receiver Out)
GPIO 4       →    DE (Driver Enable) and RE (Receiver Enable - tie together)
GND          →    GND
3.3V or 5V   →    VCC
```

### Device 2 (Slave) - ESP32 to RS485 Module
```
ESP32 Pin    →    RS485 Module
---------         -------------
GPIO 17      →    DI (Data In)
GPIO 16      →    RO (Receiver Out)
GPIO 4       →    DE (Driver Enable) and RE (Receiver Enable - tie together)
GND          →    GND
3.3V or 5V   →    VCC
```

### RS485 Bus Connection
```
Master RS485    →    Slave RS485
-------------         -------------
A               →    A
B               →    B
GND (optional)  →    GND
```

### Termination Resistors
Add 120Ω resistor between A and B on:
- Master side (first device on the bus)
- Slave side (last device on the bus)

```
     120Ω
A ----/\/\/\---- B  (on both ends)
```

## Multi-Device Setup (1 Master + Multiple Slaves)

```
           120Ω
Master ----/\/\/\----+---- Slave 1 ----+---- Slave 2 ----+---- Slave N ----/\/\/\---- 120Ω
(A/B)                |     (A/B)       |     (A/B)       |     (A/B)
                     |                 |                 |
                   Common Bus (A and B lines)
```

## Important Notes

1. **Polarity**: A and B must be connected correctly:
   - A to A
   - B to B
   - Do NOT cross them

2. **Termination**: 
   - Only at the two ends of the bus
   - Not on intermediate devices
   - Use 120Ω resistors

3. **Ground Reference**:
   - Common ground recommended for short distances
   - Isolated for long distances or different power supplies

4. **Cable Length**:
   - Maximum ~1200 meters at 100 kbps
   - Maximum ~15 meters at 10 Mbps
   - This example uses 9600 bps

5. **DE and RE pins**:
   - Must be tied together for half-duplex
   - Connected to same GPIO (RTS) on ESP32

## Common Issues

### Issue: No Communication
**Solutions**:
- Check A/B polarity
- Verify all grounds connected
- Ensure proper termination resistors
- Check baud rate matches on all devices

### Issue: Intermittent Communication
**Solutions**:
- Add termination resistors if missing
- Check cable quality and length
- Ensure proper grounding
- Reduce baud rate

### Issue: Data Corruption
**Solutions**:
- Check for EMI/noise sources
- Use twisted pair cable
- Add common mode choke
- Verify termination resistors are correct value
