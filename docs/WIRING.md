# RS485 Wiring Guide

## Connecting HT73 Temperature/Humidity Sensor

### Required Connections:

```
HT73 Sensor          ES32C14 Board
────────────────────────────────────
A+        ──────────→  A+ (terminal)
B-        ──────────→  B- (terminal)
GND       ──────────→  GND
VCC (5-24V) ─────────→ VCC (check voltage!)
```

### Wiring Diagram:

```
     HT73 Sensor                    ES32C14 Board
   ┌─────────────┐                ┌──────────────┐
   │             │                │              │
   │  A+ ────────┼────────────────┼───→ A+       │
   │  B- ────────┼────────────────┼───→ B-       │
   │  GND ───────┼────────────────┼───→ GND      │
   │  VCC ───────┼───┐            │              │
   │             │   │            └──────────────┘
   └─────────────┘   │
                     │
                 ┌───▼───┐
                 │ Power │ 5-24V DC
                 │Supply │ (check HT73 specs)
                 └───┬───┘
                     │
                    GND
```

### Important:

1. **Check HT73 voltage requirement** (5V, 12V, or 24V)
   - Some models work with 5V
   - Industrial models often need 12V or 24V
   - Check sensor label or datasheet

2. **A+ and B- polarity matters** - if not working, try swapping
   - Most common issue: reversed A+/B- wires
   - Standard: A+ = Data+, B- = Data-
   - Some manufacturers use different naming

3. **Common GND is required** between ESP32 and sensor
   - Without common GND, communication will fail
   - RS485 is differential but still needs ground reference

4. **Cable length**: 
   - Maximum 1200m for RS485 standard
   - Recommended < 100m for hobby projects
   - For long cables (>10m), use termination resistors

5. **Termination resistor** (120Ω) needed for long cables
   - Place at both ends of RS485 bus
   - Required for cable length > 10m
   - Reduces signal reflections

## Connecting Multiple Devices (Multi-Drop)

### Bus Topology:

```
                    120Ω              120Ω
ES32C14 Master ────/\/\/\────+────+────+────/\/\/\──── End of Line
    (A+/B-)                  │    │    │
                             │    │    │
                        Slave 1  │  Slave 3
                          (A+/B-)│  (A+/B-)
                                 │
                             Slave 2
                             (A+/B-)
```

### Connections:

```
Device              A+    B-    GND
─────────────────────────────────────
ES32C14 Master  →  [─────┬─────┬─] + 120Ω between A+ and B-
Slave 1         →  [     │     │ ]
Slave 2         →  [     │     │ ]
Slave 3         →  [─────┴─────┴─] + 120Ω between A+ and B-
```

### Notes for Multi-Drop:
- Connect all A+ together
- Connect all B- together
- Connect all GND together (for short distances)
- Termination only at the two ends (Master and last Slave)
- Do NOT terminate intermediate devices

## Termination Resistor Placement

### When to Use:
- Cable length > 10 meters
- High baud rates (> 19200)
- Signal quality issues
- Multiple devices on bus

### How to Install:

```
Option 1: External Resistor
A+ ────┬──── to bus
       │
      120Ω
       │
B- ────┴──── to bus
```

```
Option 2: Jumper on RS485 Module
Many RS485 modules have termination jumper:
┌────────────┐
│ [120Ω] JP1 │  ← Set jumper to enable
│  A  B      │
└────────────┘
```

### Correct Placement:

```
✓ CORRECT - Both ends terminated:
Master[120Ω]────────────────[120Ω]Last Slave

✗ WRONG - Only one end:
Master[120Ω]────────────────[ NO ]Last Slave

✗ WRONG - Middle terminated:
Master[ NO ]────[120Ω]──────[ NO ]Last Slave
```

## Cable Specifications

### Recommended Cable Types:

1. **Twisted Pair Cable** (Preferred)
   - Cat5/Cat6 Ethernet cable works well
   - Use one pair for A+/B-, another for GND
   - Built-in twisting reduces noise

2. **Shielded Cable** (For noisy environments)
   - Better for industrial settings
   - Shield should be grounded at ONE end only
   - Reduces electromagnetic interference

3. **RS485-Specific Cable**
   - 120Ω characteristic impedance
   - Available from industrial suppliers

### Cable Length vs Baud Rate:

| Baud Rate | Max Cable Length |
|-----------|------------------|
| 9600      | ~1200m          |
| 19200     | ~1000m          |
| 38400     | ~500m           |
| 115200    | ~100m           |

## Testing Your Wiring

### Test 1: Continuity Check
1. Power off all devices
2. Use multimeter to check:
   - A+ to A+ on all devices (should be continuous)
   - B- to B- on all devices (should be continuous)
   - A+ to B- (should be open circuit)

### Test 2: Polarity Check
1. Connect only ES32C14 and one sensor
2. Try communication
3. If fails, swap A+ and B- on sensor
4. Test again

### Test 3: Termination Check
1. Measure resistance between A+ and B-
2. With both 120Ω resistors: should read ~60Ω
3. With one resistor: should read ~120Ω
4. With no resistors: should be very high (>10kΩ)

## Troubleshooting Wiring Issues

### 1. No Communication at All

**Symptoms:**
- No response from sensor
- Modbus timeout errors
- Scanner finds no devices

**Solutions:**
1. **Swap A+ and B- wires** (MOST COMMON FIX!)
2. Check power supply to sensor
3. Verify GND connection
4. Check cable for breaks
5. Verify sensor is configured for correct Slave ID

### 2. Intermittent Communication

**Symptoms:**
- Sometimes works, sometimes doesn't
- Random errors
- Data corruption

**Solutions:**
1. Add 120Ω termination resistors
2. Shorten cable length
3. Use shielded twisted pair cable
4. Check for loose connections
5. Move cables away from power lines

### 3. Works with Short Cable, Fails with Long Cable

**Solutions:**
1. Add termination resistors (120Ω at both ends)
2. Reduce baud rate (try 4800 instead of 9600)
3. Use better quality cable (twisted pair)
4. Check cable length limits for your baud rate

### 4. Works on Bench, Fails in Installation

**Solutions:**
1. Check for electrical noise sources
2. Use shielded cable
3. Ensure proper grounding
4. Add common mode chokes
5. Isolate power supplies

## Wiring Best Practices

### DO:
✓ Use twisted pair cable
✓ Keep cables as short as possible
✓ Terminate both ends for cables > 10m
✓ Connect GND between all devices
✓ Use correct polarity (A+ to A+, B- to B-)
✓ Secure all connections
✓ Label wires clearly

### DON'T:
✗ Mix A+ and B- connections
✗ Run RS485 cable parallel to power cables
✗ Use telephone cable for permanent installations
✗ Terminate intermediate devices
✗ Exceed maximum cable length for baud rate
✗ Use star topology (must be bus topology)
✗ Ground shield at both ends (ground loop)

## Common Wire Color Codes

Different manufacturers use different colors:

| Standard  | A+ Color | B- Color | GND Color |
|-----------|----------|----------|-----------|
| Common #1 | Yellow   | White    | Black     |
| Common #2 | Orange   | Blue     | Black     |
| Common #3 | Red      | Green    | Black     |
| Ethernet  | Pair 1   | Pair 2   | Pair 3    |

**Always verify with multimeter - don't trust colors alone!**

## Quick Reference Diagram

```
Minimal 2-Device Setup (ES32C14 + HT73):

ES32C14                          HT73 Sensor
┌─────────┐                     ┌─────────┐
│   A+ ───┼─────────────────────┼─── A+  │
│   B- ───┼─────────────────────┼─── B-  │
│   GND ──┼─────────────────────┼─── GND │
│         │                     │        │
└─────────┘                     └────┬───┘
                                     │
                                Power Supply
                                (5-24V DC)

Optional: Add 120Ω resistor between A+ and B- on each device
```

## Additional Resources

- RS485 Standard: https://en.wikipedia.org/wiki/RS-485
- Modbus RTU Protocol: https://modbus.org/
- Cable Selection Guide: Check industrial automation suppliers
- Troubleshooting Tools: RS485 USB adapter for testing
