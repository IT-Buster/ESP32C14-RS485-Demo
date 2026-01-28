# Testing Guide - How to Test MODBUS and Manufacturer Code

[🇵🇱 Polish](#-wersja-polska) | [🇬🇧 English](#-english-version)

---

## 🇬🇧 English Version

This guide explains how to test the ES32C14 board with the manufacturer's RS485 code and MODBUS devices.

### Overview

The manufacturer (eletechsup) provides a basic RS485 echo test program that reads data from the RS485 bus and echoes it back. This guide will show you multiple ways to test this functionality and progress to MODBUS testing.

### Understanding the Manufacturer's Code

The original code from eletechsup (http://www.485io.com):

```cpp
//Author: eletechsup
//More information welcome to : http://www.485io.com 
//Arduino IDE 2.2.1
//ES32C14 Expansion Board for ESP32 38PIN BOARD
//RS485 receiving and sending test

String receivedData;
const int RS485RD = 22;

void setup() {                
  Serial.begin(115200);
  Serial.setTimeout(5);
  pinMode(RS485RD, OUTPUT);
  digitalWrite(RS485RD, LOW);
}

void loop() {  
  if (Serial.available()) {
      receivedData = Serial.readString();
      digitalWrite(RS485RD, HIGH);
      Serial.print("receivedData: ");
      Serial.println(receivedData);
      delay(100);
      digitalWrite(RS485RD, LOW);
    }
}
```

**How it works:**
- GPIO22 controls TX/RX direction (LOW = receive, HIGH = transmit)
- Receives data from RS485 bus
- Echoes it back with "receivedData: " prefix
- Uses UART0 (same as USB serial)

**Important Note:** You **cannot** use the Arduino Serial Monitor to test this code because UART0 is used for RS485! The Serial Monitor uses the same UART.

---

## Testing Methods

### Method 1: Test with USB-RS485 Adapter (Recommended for Beginners)

This is the easiest method for testing the manufacturer's code.

#### What You Need:
- 1x ES32C14 board with code uploaded
- 1x USB-RS485 adapter (CH340, FTDI, etc.)
- RS485 wiring
- Terminal program (PuTTY, Tera Term, or similar)

#### Wiring:

```
ES32C14 Terminal    →    USB-RS485 Adapter
────────────────────────────────────────────
A+                  →    A+
B-                  →    B-
GND                 →    GND
```

#### Step-by-Step Testing:

1. **Upload the manufacturer's code** to ES32C14:
   - Open `examples/01_BasicRS485_Official/BasicRS485_Official.ino`
   - Upload to ES32C14
   - Disconnect USB after upload completes

2. **Connect the USB-RS485 adapter:**
   - Wire A+, B-, and GND as shown above
   - Connect adapter to your computer

3. **Open terminal program:**
   - Windows: Use PuTTY or Tera Term
   - Linux/Mac: Use `screen` or `minicom`
   - Set port to USB-RS485 adapter port
   - **Baudrate: 115200** (must match the code!)
   - Data bits: 8, Parity: None, Stop bits: 1

4. **Power the ES32C14:**
   - Reconnect USB power (or use external 5V power)
   - Do NOT open Arduino Serial Monitor!

5. **Send test data:**
   - Type any text in the terminal and press Enter
   - You should see: `receivedData: your text here`

**Expected Results:**
```
Input:  Hello
Output: receivedData: Hello

Input:  Test123
Output: receivedData: Test123
```

**Troubleshooting:**
- No response? → Try swapping A+ and B- wires
- Garbled text? → Check baudrate is 115200 on both sides
- Random characters? → Check GND connection

---

### Method 2: Test with Two ES32C14 Boards (Best for Full Testing)

This method uses two ES32C14 boards to create a complete RS485 network.

#### What You Need:
- 2x ES32C14 boards
- RS485 wiring
- 2x USB cables (for power)

#### Setup:

**Board 1: Transmitter (uses modified code)**
```cpp
// Upload this to Board 1
const int RS485RD = 22;

void setup() {
  Serial.begin(115200);
  pinMode(RS485RD, OUTPUT);
  pinMode(2, OUTPUT);  // Built-in LED
}

void loop() {
  digitalWrite(RS485RD, HIGH);  // Transmit mode
  digitalWrite(2, HIGH);        // LED on
  Serial.println("Test message from Board 1");
  delay(100);
  digitalWrite(RS485RD, LOW);   // Receive mode
  digitalWrite(2, LOW);         // LED off
  delay(2000);                  // Wait 2 seconds
}
```

**Board 2: Receiver (use manufacturer's original code)**
- Upload `examples/01_BasicRS485_Official/BasicRS485_Official.ino`

#### Wiring:

```
Board 1 (TX)         →    Board 2 (RX)
─────────────────────────────────────
A+                   →    A+
B-                   →    B-
GND                  →    GND
```

#### Testing:

1. Upload transmitter code to Board 1
2. Upload manufacturer's code to Board 2
3. Connect A+, B-, and GND between boards
4. Power both boards (can use USB for power)

**Expected Results:**
- Board 1 LED blinks every 2 seconds
- Board 2 receives: "receivedData: Test message from Board 1" every 2 seconds

**How to Verify:**
Since you can't use Serial Monitor, use one of these methods:
- Add LED blink patterns to Board 2 (blink on receive)
- Use Serial2 for debug output (GPIO16/17)
- Use WiFi to send status to web interface

---

### Method 3: Test with Serial2 Debug Output (Advanced)

This method adds debug output while keeping RS485 working.

#### Modified Code:

```cpp
//Author: eletechsup with debug modifications
//RS485 receiving and sending test with Serial2 debug

String receivedData;
const int RS485RD = 22;

void setup() {
  // RS485 on Serial (UART0)
  Serial.begin(115200);
  Serial.setTimeout(5);
  
  // Debug output on Serial2 (UART2)
  Serial2.begin(115200, SERIAL_8N1, 16, 17);  // RX=GPIO16, TX=GPIO17
  Serial2.println("RS485 Test Started - Debug Output");
  
  pinMode(RS485RD, OUTPUT);
  digitalWrite(RS485RD, LOW);
  pinMode(2, OUTPUT);  // Built-in LED
}

void loop() {
  if (Serial.available()) {
    receivedData = Serial.readString();
    
    // Debug to Serial2
    Serial2.print("Received: ");
    Serial2.println(receivedData);
    
    // Blink LED
    digitalWrite(2, HIGH);
    
    // Echo to RS485
    digitalWrite(RS485RD, HIGH);
    Serial.print("receivedData: ");
    Serial.println(receivedData);
    delay(100);
    digitalWrite(RS485RD, LOW);
    
    digitalWrite(2, LOW);
  }
}
```

#### Wiring for Debug:

Connect USB-TTL adapter to Serial2:
```
USB-TTL Adapter      →    ES32C14
───────────────────────────────────
RX                   →    GPIO17 (TX2)
TX                   →    GPIO16 (RX2)
GND                  →    GND
```

Now you can monitor debug output while RS485 communication runs!

---

## MODBUS Testing

### What is MODBUS?

MODBUS is a communication protocol commonly used in industrial systems. It runs on top of RS485 and uses a master-slave architecture.

### Prerequisites:

Before testing MODBUS:
1. ✅ Verify basic RS485 works (use methods above)
2. ✅ Install ModbusMaster library in Arduino IDE
3. ✅ Know your MODBUS device specifications:
   - Slave ID (address)
   - Baudrate (usually 9600)
   - Register addresses you want to read

### MODBUS Test Method 1: Read HT73 Sensor

The HT73 is a common temperature/humidity sensor with MODBUS support.

#### What You Need:
- ES32C14 board
- HT73 sensor (or compatible MODBUS sensor)
- Power supply for sensor (5V, 12V, or 24V - check sensor specs)

#### Wiring:

```
HT73 Sensor          →    ES32C14
───────────────────────────────────
A+                   →    A+ (terminal)
B-                   →    B- (terminal)
GND                  →    GND
VCC                  →    External power supply
```

#### Code to Upload:

Use the provided example:
`examples/03_ModbusRTU_Master/ModbusRTU_Master.ino`

This example:
- Reads temperature from register 0
- Reads humidity from register 1
- Blinks LED to show status:
  - Fast blink = communicating
  - 3 blinks = success
  - 5 blinks = error

#### Testing Steps:

1. **Configure the example:**
   ```cpp
   // In ModbusRTU_Master.ino, check these values:
   #define SLAVE_ID 5         // HT73 usually uses ID 5
   Serial.begin(9600, SERIAL_8N1);  // Standard MODBUS baudrate
   ```

2. **Upload and power:**
   - Upload the code to ES32C14
   - Power the HT73 sensor
   - ES32C14 can be powered via USB

3. **Observe LED patterns:**
   - LED blinks every ~5 seconds = Reading sensor
   - 3 blinks = Successful read
   - 5 blinks = Communication error

4. **View results (optional):**
   - Add Serial2 debug output (see Method 3 above)
   - Or use WiFi scanner (see next method)

**Troubleshooting:**
- 5 blinks (error)? → Swap A+ and B- wires (most common!)
- Still error? → Try different Slave ID (1, 5, or use scanner)
- Timeout error? → Check baudrate and sensor power
- Wrong values? → HT73 values need to be divided by 10

---

### MODBUS Test Method 2: Use MODBUS Scanner (Best for Unknown Devices)

When you don't know the Slave ID or register addresses, use the scanner!

#### What to Use:

`examples/04_ModbusRTU_Scanner/ModbusRTU_Scanner.ino`

This example:
- Creates WiFi Access Point: "ES32C14-Scanner"
- Scans MODBUS addresses 1-10 (configurable to 1-247)
- Shows results in web browser
- Tests both FC03 (Holding Registers) and FC04 (Input Registers)

#### Testing Steps:

1. **Upload the scanner:**
   - Open `examples/04_ModbusRTU_Scanner/ModbusRTU_Scanner.ino`
   - Upload to ES32C14

2. **Connect your MODBUS device:**
   - Wire A+, B-, GND as usual
   - Power the device

3. **Connect to WiFi:**
   - Look for WiFi network: "ES32C14-Scanner"
   - Password: "12345678"
   - Connect with phone or laptop

4. **Open web interface:**
   - Open browser
   - Go to: `http://192.168.4.1`
   - Click "Start Scan"

5. **View results:**
   - Scanner will test addresses 1-10
   - Green = Device found at this address
   - Shows which function codes (FC03/FC04) work
   - Takes ~2 minutes to complete

**Example Results:**
```
Scan Complete!
Found devices at addresses:
- Address 5: FC03 ✓, FC04 ✓
- Address 8: FC03 ✓, FC04 ✗

Configure your master to use:
- Slave ID: 5
- Function Code: 03 (Read Holding Registers)
```

---

### MODBUS Test Method 3: Test with PC Software (Most Reliable)

Use PC software to verify your MODBUS device before testing with ES32C14.

#### What You Need:
- USB-RS485 adapter
- MODBUS software (choose one):
  - QModMaster (free, Windows/Linux/Mac)
  - ModScan (free, Windows)
  - Modbus Poll (paid, Windows)

#### Steps:

1. **Connect device to PC:**
   ```
   MODBUS Device       →    USB-RS485
   ────────────────────────────────────
   A+                  →    A+
   B-                  →    B-
   GND                 →    GND
   ```

2. **Configure software:**
   - Port: Your USB-RS485 COM port
   - Baudrate: 9600 (or check device manual)
   - Parity: None
   - Data bits: 8
   - Stop bits: 1

3. **Scan for devices:**
   - Most software has "Scan" function
   - Scan Slave IDs 1-247
   - Note which addresses respond

4. **Read registers:**
   - Once you find a device, try reading registers
   - Function Code 03 (Read Holding Registers)
   - Start address: 0
   - Quantity: 2 or 10 registers

5. **Record settings:**
   - Note the Slave ID that works
   - Note which registers contain data
   - Use these settings with ES32C14

**Now you know:**
- ✅ Device works and MODBUS is configured correctly
- ✅ Correct Slave ID
- ✅ Correct baudrate
- ✅ Which registers to read

Use these exact settings in your ES32C14 code!

---

## Common Testing Issues

### Issue 1: "Serial Monitor is blank"

**Why:** The manufacturer's code uses UART0 for RS485, which is the same UART used by USB. You cannot use Serial Monitor!

**Solutions:**
1. Use Method 1 (USB-RS485 adapter) for testing
2. Add Serial2 debug output (see Method 3)
3. Use LED blink patterns
4. Use WiFi web interface (Example 04)

### Issue 2: "No response from RS485"

**Checklist:**
- ✅ Are A+ and B- wired correctly? (Try swapping!)
- ✅ Is GND connected between devices?
- ✅ Is the other device powered on?
- ✅ Are both sides using same baudrate?
- ✅ Is GPIO22 set correctly (LOW for receive)?

**Test Steps:**
1. Try swapping A+ and B- (fixes 80% of issues)
2. Check with multimeter:
   - A-B voltage should be ~0V idle, ±2-5V when transmitting
   - Check continuity of wires
3. Test with known-good device
4. Try different baudrate (9600 instead of 115200)

### Issue 3: "MODBUS timeout error (0xE2)"

**Causes:**
- Wrong Slave ID
- Swapped A+/B- wires
- Wrong baudrate
- Device not powered
- Device in wrong mode

**Solutions:**
1. **Use the scanner** (`examples/04_ModbusRTU_Scanner/`) to find correct address
2. **Swap A+ and B-** wires
3. **Verify device settings:**
   - Check if device needs to be configured for MODBUS mode
   - Some devices have DIP switches or configuration software
4. **Test with PC software first** (see Method 3 above)

### Issue 4: "Wrong temperature/humidity values"

**For HT73 sensor:**
- Values need to be **divided by 10**
- Register 0 = 252 means 25.2°C
- Register 1 = 651 means 65.1% humidity

**Code fix:**
```cpp
// In ModbusRTU_Master.ino, modify:
float temperature = node.getResponseBuffer(0) / 10.0;  // Divide by 10!
float humidity = node.getResponseBuffer(1) / 10.0;     // Divide by 10!
```

### Issue 5: "ES32C14 crashes during scan"

**Why:** Scanning all 247 addresses can trigger watchdog timer.

**Solutions:**
1. Reduce scan range:
   ```cpp
   // In ModbusRTU_Scanner.ino:
   for (uint8_t id = 1; id <= 10; id++)  // Instead of 247
   ```

2. Add delays:
   ```cpp
   delay(200);  // Between each scan
   yield();     // Allow other tasks to run
   ```

3. Increase timeout:
   ```cpp
   node.setTimeout(2000);  // 2 seconds
   ```

---

## Testing Checklist

Before reporting issues, verify:

### Basic RS485 Test:
- [ ] Uploaded manufacturer's code correctly
- [ ] Wired A+, B-, GND correctly
- [ ] Tried swapping A+ and B-
- [ ] Both devices using same baudrate
- [ ] Both devices powered on
- [ ] GND connected between devices
- [ ] Not using Serial Monitor for testing

### MODBUS Test:
- [ ] Basic RS485 works (tested above)
- [ ] ModbusMaster library installed
- [ ] Know device Slave ID (or used scanner)
- [ ] Know correct baudrate (usually 9600)
- [ ] Device is powered with correct voltage
- [ ] Tried swapping A+ and B-
- [ ] Tested device with PC software first (recommended)

### For Advanced Users:
- [ ] Added termination resistors (120Ω) if cable > 3m
- [ ] Checked signal quality with oscilloscope
- [ ] Verified power supply voltage and current
- [ ] Checked for ground loops
- [ ] Tested in both directions (TX and RX)

---

## Quick Reference

### Baudrates:
- Manufacturer's basic code: **115200**
- Standard MODBUS: **9600**
- Also try: 4800, 19200

### Common Slave IDs:
- HT73 sensor: **5**
- Many devices: **1**
- Try scanning: **1-10**

### GPIO Pins:
- GPIO22: Direction control (DE/RE)
- GPIO2: Built-in LED
- GPIO16: Serial2 RX (for debug)
- GPIO17: Serial2 TX (for debug)

### Example Files:
1. `01_BasicRS485_Official/` - Manufacturer's original code
2. `02_BasicRS485_Echo/` - Enhanced with LED and auto-send
3. `03_ModbusRTU_Master/` - Read HT73 sensor
4. `04_ModbusRTU_Scanner/` - Find unknown devices

---

## Need More Help?

1. **Read other documentation:**
   - [WIRING.md](WIRING.md) - Detailed wiring diagrams
   - [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - More troubleshooting tips
   - [PINOUT.md](PINOUT.md) - ES32C14 pin reference

2. **Check examples:**
   - All examples include detailed comments
   - Start with simpler examples first

3. **Ask for help:**
   - GitHub Issues: Report bugs or ask questions
   - GitHub Discussions: General questions and community help

---

## 🇵🇱 Wersja Polska

Ten przewodnik wyjaśnia, jak testować płytkę ES32C14 z kodem RS485 producenta i urządzeniami MODBUS.

### Przegląd

Producent (eletechsup) dostarcza podstawowy program testowy RS485, który odczytuje dane z magistrali RS485 i odsyła je z powrotem (echo). Ten przewodnik pokaże Ci wiele sposobów testowania tej funkcjonalności oraz postęp do testowania MODBUS.

### Zrozumienie kodu producenta

Oryginalny kod od eletechsup (http://www.485io.com):

```cpp
//Author: eletechsup
//More information welcome to : http://www.485io.com 
//Arduino IDE 2.2.1
//ES32C14 Expansion Board for ESP32 38PIN BOARD
//RS485 receiving and sending test

String receivedData;
const int RS485RD = 22;

void setup() {                
  Serial.begin(115200);
  Serial.setTimeout(5);
  pinMode(RS485RD, OUTPUT);
  digitalWrite(RS485RD, LOW);
}

void loop() {  
  if (Serial.available()) {
      receivedData = Serial.readString();
      digitalWrite(RS485RD, HIGH);
      Serial.print("receivedData: ");
      Serial.println(receivedData);
      delay(100);
      digitalWrite(RS485RD, LOW);
    }
}
```

**Jak to działa:**
- GPIO22 kontroluje kierunek TX/RX (LOW = odbiór, HIGH = transmisja)
- Odbiera dane z magistrali RS485
- Odsyła je z powrotem z przedrostkiem "receivedData: "
- Używa UART0 (tego samego co USB serial)

**Ważna uwaga:** **Nie możesz** użyć Monitora Szeregowego Arduino do testowania tego kodu, ponieważ UART0 jest używany dla RS485! Monitor Szeregowy używa tego samego UART.

---

## Metody testowania

### Metoda 1: Test z adapterem USB-RS485 (zalecane dla początkujących)

To najłatwiejsza metoda testowania kodu producenta.

#### Czego potrzebujesz:
- 1x płytka ES32C14 z wgranym kodem
- 1x adapter USB-RS485 (CH340, FTDI, itp.)
- Okablowanie RS485
- Program terminalowy (PuTTY, Tera Term lub podobny)

#### Okablowanie:

```
Terminal ES32C14    →    Adapter USB-RS485
────────────────────────────────────────────
A+                  →    A+
B-                  →    B-
GND                 →    GND
```

#### Testowanie krok po kroku:

1. **Wgraj kod producenta** na ES32C14:
   - Otwórz `examples/01_BasicRS485_Official/BasicRS485_Official.ino`
   - Wgraj na ES32C14
   - Odłącz USB po zakończeniu wgrywania

2. **Podłącz adapter USB-RS485:**
   - Podłącz przewody A+, B- i GND jak pokazano powyżej
   - Podłącz adapter do komputera

3. **Otwórz program terminalowy:**
   - Windows: Użyj PuTTY lub Tera Term
   - Linux/Mac: Użyj `screen` lub `minicom`
   - Ustaw port na port adaptera USB-RS485
   - **Prędkość: 115200** (musi być zgodna z kodem!)
   - Bity danych: 8, Parzystość: Brak, Bity stopu: 1

4. **Zasil ES32C14:**
   - Podłącz ponownie zasilanie USB (lub użyj zewnętrznego zasilania 5V)
   - NIE otwieraj Monitora Szeregowego Arduino!

5. **Wyślij dane testowe:**
   - Wpisz dowolny tekst w terminalu i naciśnij Enter
   - Powinieneś zobaczyć: `receivedData: twój tekst tutaj`

**Oczekiwane rezultaty:**
```
Wejście:  Hello
Wyjście:  receivedData: Hello

Wejście:  Test123
Wyjście:  receivedData: Test123
```

**Rozwiązywanie problemów:**
- Brak odpowiedzi? → Spróbuj zamienić przewody A+ i B-
- Nieczytelny tekst? → Sprawdź czy prędkość to 115200 po obu stronach
- Losowe znaki? → Sprawdź połączenie GND

---

### Metoda 2: Test z dwiema płytkami ES32C14 (najlepsze do pełnego testowania)

Ta metoda używa dwóch płytek ES32C14 do utworzenia kompletnej sieci RS485.

#### Czego potrzebujesz:
- 2x płytki ES32C14
- Okablowanie RS485
- 2x kable USB (do zasilania)

#### Konfiguracja:

**Płytka 1: Nadajnik (użyj zmodyfikowanego kodu)**
```cpp
// Wgraj to na Płytkę 1
const int RS485RD = 22;

void setup() {
  Serial.begin(115200);
  pinMode(RS485RD, OUTPUT);
  pinMode(2, OUTPUT);  // Wbudowana LED
}

void loop() {
  digitalWrite(RS485RD, HIGH);  // Tryb transmisji
  digitalWrite(2, HIGH);        // LED włączona
  Serial.println("Wiadomość testowa z Płytki 1");
  delay(100);
  digitalWrite(RS485RD, LOW);   // Tryb odbioru
  digitalWrite(2, LOW);         // LED wyłączona
  delay(2000);                  // Czekaj 2 sekundy
}
```

**Płytka 2: Odbiornik (użyj oryginalnego kodu producenta)**
- Wgraj `examples/01_BasicRS485_Official/BasicRS485_Official.ino`

#### Okablowanie:

```
Płytka 1 (TX)        →    Płytka 2 (RX)
─────────────────────────────────────────
A+                   →    A+
B-                   →    B-
GND                  →    GND
```

#### Testowanie:

1. Wgraj kod nadajnika na Płytkę 1
2. Wgraj kod producenta na Płytkę 2
3. Podłącz A+, B- i GND między płytkami
4. Zasil obie płytki (można użyć USB do zasilania)

**Oczekiwane rezultaty:**
- LED na Płytce 1 miga co 2 sekundy
- Płytka 2 odbiera: "receivedData: Wiadomość testowa z Płytki 1" co 2 sekundy

**Jak zweryfikować:**
Ponieważ nie możesz użyć Monitora Szeregowego, użyj jednej z tych metod:
- Dodaj wzory migania LED na Płytce 2 (miga przy odbiorze)
- Użyj Serial2 do wyjścia debugowania (GPIO16/17)
- Użyj WiFi do wysyłania statusu do interfejsu WWW

---

### Metoda 3: Test z wyjściem debugowania Serial2 (zaawansowane)

Ta metoda dodaje wyjście debugowania przy zachowaniu działającego RS485.

#### Zmodyfikowany kod:

```cpp
//Author: eletechsup z modyfikacjami debug
//RS485 test odbierania i wysyłania z debugowaniem Serial2

String receivedData;
const int RS485RD = 22;

void setup() {
  // RS485 na Serial (UART0)
  Serial.begin(115200);
  Serial.setTimeout(5);
  
  // Wyjście debug na Serial2 (UART2)
  Serial2.begin(115200, SERIAL_8N1, 16, 17);  // RX=GPIO16, TX=GPIO17
  Serial2.println("Test RS485 rozpoczęty - Wyjście Debug");
  
  pinMode(RS485RD, OUTPUT);
  digitalWrite(RS485RD, LOW);
  pinMode(2, OUTPUT);  // Wbudowana LED
}

void loop() {
  if (Serial.available()) {
    receivedData = Serial.readString();
    
    // Debug do Serial2
    Serial2.print("Odebrano: ");
    Serial2.println(receivedData);
    
    // Migaj LED
    digitalWrite(2, HIGH);
    
    // Echo do RS485
    digitalWrite(RS485RD, HIGH);
    Serial.print("receivedData: ");
    Serial.println(receivedData);
    delay(100);
    digitalWrite(RS485RD, LOW);
    
    digitalWrite(2, LOW);
  }
}
```

#### Okablowanie dla debugowania:

Podłącz adapter USB-TTL do Serial2:
```
Adapter USB-TTL      →    ES32C14
───────────────────────────────────
RX                   →    GPIO17 (TX2)
TX                   →    GPIO16 (RX2)
GND                  →    GND
```

Teraz możesz monitorować wyjście debugowania podczas działania komunikacji RS485!

---

## Testowanie MODBUS

### Co to jest MODBUS?

MODBUS to protokół komunikacyjny powszechnie używany w systemach przemysłowych. Działa na RS485 i używa architektury master-slave (główny-podrzędny).

### Wymagania wstępne:

Przed testowaniem MODBUS:
1. ✅ Zweryfikuj, że podstawowe RS485 działa (użyj metod powyżej)
2. ✅ Zainstaluj bibliotekę ModbusMaster w Arduino IDE
3. ✅ Poznaj specyfikacje swojego urządzenia MODBUS:
   - Slave ID (adres)
   - Prędkość transmisji (zwykle 9600)
   - Adresy rejestrów, które chcesz odczytać

### Metoda testowania MODBUS 1: Odczyt czujnika HT73

HT73 to popularny czujnik temperatury/wilgotności z obsługą MODBUS.

#### Czego potrzebujesz:
- Płytka ES32C14
- Czujnik HT73 (lub kompatybilny czujnik MODBUS)
- Zasilacz dla czujnika (5V, 12V lub 24V - sprawdź specyfikację czujnika)

#### Okablowanie:

```
Czujnik HT73         →    ES32C14
───────────────────────────────────
A+                   →    A+ (terminal)
B-                   →    B- (terminal)
GND                  →    GND
VCC                  →    Zewnętrzny zasilacz
```

#### Kod do wgrania:

Użyj dostarczonego przykładu:
`examples/03_ModbusRTU_Master/ModbusRTU_Master.ino`

Ten przykład:
- Odczytuje temperaturę z rejestru 0
- Odczytuje wilgotność z rejestru 1
- Miga LED aby pokazać status:
  - Szybkie miganie = komunikacja
  - 3 mignięcia = sukces
  - 5 mignięć = błąd

#### Kroki testowania:

1. **Skonfiguruj przykład:**
   ```cpp
   // W ModbusRTU_Master.ino, sprawdź te wartości:
   #define SLAVE_ID 5         // HT73 zazwyczaj używa ID 5
   Serial.begin(9600, SERIAL_8N1);  // Standardowa prędkość MODBUS
   ```

2. **Wgraj i zasil:**
   - Wgraj kod na ES32C14
   - Zasil czujnik HT73
   - ES32C14 może być zasilany przez USB

3. **Obserwuj wzory LED:**
   - LED miga co ~5 sekund = Odczyt czujnika
   - 3 mignięcia = Udany odczyt
   - 5 mignięć = Błąd komunikacji

4. **Zobacz wyniki (opcjonalnie):**
   - Dodaj wyjście debugowania Serial2 (patrz Metoda 3 powyżej)
   - Lub użyj skanera WiFi (patrz następna metoda)

**Rozwiązywanie problemów:**
- 5 mignięć (błąd)? → Zamień przewody A+ i B- (najczęstsze!)
- Wciąż błąd? → Spróbuj innego Slave ID (1, 5 lub użyj skanera)
- Błąd timeout? → Sprawdź prędkość i zasilanie czujnika
- Złe wartości? → Wartości HT73 trzeba podzielić przez 10

---

### Metoda testowania MODBUS 2: Użyj skanera MODBUS (najlepsze dla nieznanych urządzeń)

Gdy nie znasz Slave ID lub adresów rejestrów, użyj skanera!

#### Czego użyć:

`examples/04_ModbusRTU_Scanner/ModbusRTU_Scanner.ino`

Ten przykład:
- Tworzy punkt dostępowy WiFi: "ES32C14-Scanner"
- Skanuje adresy MODBUS 1-10 (konfigurowalne do 1-247)
- Pokazuje wyniki w przeglądarce internetowej
- Testuje zarówno FC03 (rejestry przechowywania) jak i FC04 (rejestry wejściowe)

#### Kroki testowania:

1. **Wgraj skaner:**
   - Otwórz `examples/04_ModbusRTU_Scanner/ModbusRTU_Scanner.ino`
   - Wgraj na ES32C14

2. **Podłącz swoje urządzenie MODBUS:**
   - Podłącz przewody A+, B-, GND jak zwykle
   - Zasil urządzenie

3. **Połącz się z WiFi:**
   - Poszukaj sieci WiFi: "ES32C14-Scanner"
   - Hasło: "12345678"
   - Połącz się telefonem lub laptopem

4. **Otwórz interfejs WWW:**
   - Otwórz przeglądarkę
   - Przejdź do: `http://192.168.4.1`
   - Kliknij "Start Scan"

5. **Zobacz wyniki:**
   - Skaner przetestuje adresy 1-10
   - Zielony = Urządzenie znalezione pod tym adresem
   - Pokazuje które kody funkcji (FC03/FC04) działają
   - Zajmuje ~2 minuty

**Przykładowe rezultaty:**
```
Skanowanie zakończone!
Znaleziono urządzenia pod adresami:
- Adres 5: FC03 ✓, FC04 ✓
- Adres 8: FC03 ✓, FC04 ✗

Skonfiguruj swojego mastera aby używał:
- Slave ID: 5
- Kod funkcji: 03 (Odczyt rejestrów przechowywania)
```

---

### Metoda testowania MODBUS 3: Test z oprogramowaniem PC (najbardziej niezawodne)

Użyj oprogramowania PC aby zweryfikować swoje urządzenie MODBUS przed testowaniem z ES32C14.

#### Czego potrzebujesz:
- Adapter USB-RS485
- Oprogramowanie MODBUS (wybierz jedno):
  - QModMaster (darmowe, Windows/Linux/Mac)
  - ModScan (darmowe, Windows)
  - Modbus Poll (płatne, Windows)

#### Kroki:

1. **Podłącz urządzenie do PC:**
   ```
   Urządzenie MODBUS    →    USB-RS485
   ────────────────────────────────────
   A+                   →    A+
   B-                   →    B-
   GND                  →    GND
   ```

2. **Skonfiguruj oprogramowanie:**
   - Port: Twój port COM USB-RS485
   - Prędkość: 9600 (lub sprawdź instrukcję urządzenia)
   - Parzystość: Brak
   - Bity danych: 8
   - Bity stopu: 1

3. **Skanuj urządzenia:**
   - Większość programów ma funkcję "Scan"
   - Skanuj Slave ID 1-247
   - Zanotuj które adresy odpowiadają

4. **Odczytaj rejestry:**
   - Gdy znajdziesz urządzenie, spróbuj odczytać rejestry
   - Kod funkcji 03 (Odczyt rejestrów przechowywania)
   - Adres startowy: 0
   - Ilość: 2 lub 10 rejestrów

5. **Zapisz ustawienia:**
   - Zanotuj Slave ID, które działa
   - Zanotuj które rejestry zawierają dane
   - Użyj tych ustawień z ES32C14

**Teraz wiesz:**
- ✅ Urządzenie działa i MODBUS jest poprawnie skonfigurowany
- ✅ Prawidłowy Slave ID
- ✅ Prawidłowa prędkość transmisji
- ✅ Które rejestry odczytać

Użyj dokładnie tych samych ustawień w kodzie ES32C14!

---

## Typowe problemy przy testowaniu

### Problem 1: "Monitor Szeregowy jest pusty"

**Dlaczego:** Kod producenta używa UART0 dla RS485, który jest tym samym UART używanym przez USB. Nie możesz użyć Monitora Szeregowego!

**Rozwiązania:**
1. Użyj Metody 1 (adapter USB-RS485) do testowania
2. Dodaj wyjście debugowania Serial2 (patrz Metoda 3)
3. Użyj wzorów migania LED
4. Użyj interfejsu WWW przez WiFi (Przykład 04)

### Problem 2: "Brak odpowiedzi z RS485"

**Lista kontrolna:**
- ✅ Czy A+ i B- są poprawnie podłączone? (Spróbuj zamienić!)
- ✅ Czy GND jest połączony między urządzeniami?
- ✅ Czy drugie urządzenie jest włączone?
- ✅ Czy obie strony używają tej samej prędkości?
- ✅ Czy GPIO22 jest poprawnie ustawiony (LOW dla odbioru)?

**Kroki testowe:**
1. Spróbuj zamienić A+ i B- (naprawia 80% problemów)
2. Sprawdź multimetrem:
   - Napięcie A-B powinno być ~0V w spoczynku, ±2-5V podczas transmisji
   - Sprawdź ciągłość przewodów
3. Testuj ze sprawdzonym urządzeniem
4. Spróbuj innej prędkości (9600 zamiast 115200)

### Problem 3: "Błąd timeout MODBUS (0xE2)"

**Przyczyny:**
- Zły Slave ID
- Zamienione przewody A+/B-
- Zła prędkość transmisji
- Urządzenie nie zasilone
- Urządzenie w złym trybie

**Rozwiązania:**
1. **Użyj skanera** (`examples/04_ModbusRTU_Scanner/`) aby znaleźć prawidłowy adres
2. **Zamień przewody A+ i B-**
3. **Zweryfikuj ustawienia urządzenia:**
   - Sprawdź czy urządzenie musi być skonfigurowane w tryb MODBUS
   - Niektóre urządzenia mają przełączniki DIP lub oprogramowanie konfiguracyjne
4. **Testuj najpierw z oprogramowaniem PC** (patrz Metoda 3 powyżej)

### Problem 4: "Złe wartości temperatury/wilgotności"

**Dla czujnika HT73:**
- Wartości trzeba **podzielić przez 10**
- Rejestr 0 = 252 oznacza 25.2°C
- Rejestr 1 = 651 oznacza 65.1% wilgotności

**Poprawka w kodzie:**
```cpp
// W ModbusRTU_Master.ino, zmodyfikuj:
float temperature = node.getResponseBuffer(0) / 10.0;  // Podziel przez 10!
float humidity = node.getResponseBuffer(1) / 10.0;     // Podziel przez 10!
```

### Problem 5: "ES32C14 zawiesza się podczas skanowania"

**Dlaczego:** Skanowanie wszystkich 247 adresów może wywołać watchdog timer.

**Rozwiązania:**
1. Zmniejsz zakres skanowania:
   ```cpp
   // W ModbusRTU_Scanner.ino:
   for (uint8_t id = 1; id <= 10; id++)  // Zamiast 247
   ```

2. Dodaj opóźnienia:
   ```cpp
   delay(200);  // Między każdym skanem
   yield();     // Pozwól innym zadaniom działać
   ```

3. Zwiększ timeout:
   ```cpp
   node.setTimeout(2000);  // 2 sekundy
   ```

---

## Lista kontrolna testowania

Przed zgłaszaniem problemów, zweryfikuj:

### Test podstawowego RS485:
- [ ] Poprawnie wgrałem kod producenta
- [ ] Poprawnie podłączyłem A+, B-, GND
- [ ] Próbowałem zamienić A+ i B-
- [ ] Oba urządzenia używają tej samej prędkości
- [ ] Oba urządzenia są włączone
- [ ] GND jest połączony między urządzeniami
- [ ] Nie używam Monitora Szeregowego do testowania

### Test MODBUS:
- [ ] Podstawowe RS485 działa (przetestowane powyżej)
- [ ] Biblioteka ModbusMaster jest zainstalowana
- [ ] Znam Slave ID urządzenia (lub użyłem skanera)
- [ ] Znam prawidłową prędkość (zwykle 9600)
- [ ] Urządzenie jest zasilone prawidłowym napięciem
- [ ] Próbowałem zamienić A+ i B-
- [ ] Przetestowałem urządzenie z oprogramowaniem PC (zalecane)

### Dla zaawansowanych:
- [ ] Dodałem rezystory terminujące (120Ω) jeśli kabel > 3m
- [ ] Sprawdziłem jakość sygnału oscyloskopem
- [ ] Zweryfikowałem napięcie i prąd zasilacza
- [ ] Sprawdziłem pętle uziemienia
- [ ] Testowałem w obu kierunkach (TX i RX)

---

## Krótki przewodnik

### Prędkości transmisji:
- Podstawowy kod producenta: **115200**
- Standardowy MODBUS: **9600**
- Spróbuj też: 4800, 19200

### Popularne Slave ID:
- Czujnik HT73: **5**
- Wiele urządzeń: **1**
- Spróbuj skanować: **1-10**

### Piny GPIO:
- GPIO22: Kontrola kierunku (DE/RE)
- GPIO2: Wbudowana LED
- GPIO16: Serial2 RX (do debugowania)
- GPIO17: Serial2 TX (do debugowania)

### Pliki przykładów:
1. `01_BasicRS485_Official/` - Oryginalny kod producenta
2. `02_BasicRS485_Echo/` - Rozszerzony z LED i auto-wysyłaniem
3. `03_ModbusRTU_Master/` - Odczyt czujnika HT73
4. `04_ModbusRTU_Scanner/` - Znajdź nieznane urządzenia

---

## Potrzebujesz więcej pomocy?

1. **Przeczytaj inną dokumentację:**
   - [WIRING.md](WIRING.md) - Szczegółowe schematy okablowania
   - [TROUBLESHOOTING.md](TROUBLESHOOTING.md) - Więcej wskazówek rozwiązywania problemów
   - [PINOUT.md](PINOUT.md) - Opis wyprowadzeń ES32C14

2. **Sprawdź przykłady:**
   - Wszystkie przykłady zawierają szczegółowe komentarze
   - Zacznij od prostszych przykładów

3. **Poproś o pomoc:**
   - GitHub Issues: Zgłaszaj błędy lub zadawaj pytania
   - GitHub Discussions: Ogólne pytania i pomoc społeczności

---

**Stworzone z ❤️ dla społeczności ESP32 i RS485/Modbus**
