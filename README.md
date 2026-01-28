# ES32C14 RS485/Modbus RTU Demo Project

**Complete demo project for ES32C14 board with RS485/Modbus RTU communication examples and comprehensive documentation.**

[🇵🇱 Polish](#-wersja-polska) | [🇬🇧 English](#-english-version)

---

## 🇬🇧 English Version

### Overview

This repository provides a complete set of Arduino examples and documentation for the **ES32C14 Expansion Board** - an ESP32-based board with built-in RS485 transceiver. Perfect for industrial communication, sensor networks, and Modbus RTU applications.

### Features

✅ **4 Complete Arduino Examples** - From basic RS485 to advanced Modbus scanner  
✅ **Comprehensive Documentation** - Pinout, wiring, troubleshooting guides  
✅ **Beginner-Friendly** - Detailed comments and step-by-step instructions  
✅ **Real-World Applications** - HT73 sensor reading, device scanning  
✅ **WiFi Integration** - Web-based monitoring and control  
✅ **Bilingual Docs** - Polish and English documentation

### What's Included

```
ESP32C14-RS485-Demo/
├── examples/
│   ├── 01_BasicRS485_Official/     # Original eletechsup example
│   ├── 02_BasicRS485_Echo/         # Enhanced echo test with LED
│   ├── 03_ModbusRTU_Master/        # Read HT73 sensor via Modbus
│   └── 04_ModbusRTU_Scanner/       # Scan for Modbus devices (WiFi web UI)
├── docs/
│   ├── PINOUT.md                   # ES32C14 pin reference
│   ├── WIRING.md                   # RS485 wiring guide
│   └── TROUBLESHOOTING.md          # Common issues & solutions
├── hardware/
│   └── README.md                   # Hardware specifications
└── README.md                       # This file
```

### Quick Start

#### Hardware Requirements

1. **ES32C14 Expansion Board** (with ESP32 module)
2. **USB Cable** (Micro-USB or USB-C)
3. **RS485 Device** (e.g., HT73 sensor, another ES32C14, or USB-RS485 adapter)
4. **Jumper Wires** and/or RS485 cable
5. **120Ω Resistors** (optional, for cables >10m)

#### Software Requirements

1. **Arduino IDE** 2.0 or later
2. **ESP32 Board Package**:
   - File → Preferences → Additional Board Manager URLs:
   - `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Tools → Board → Boards Manager → Search "ESP32" → Install
3. **ModbusMaster Library** (for examples 3 & 4):
   - Tools → Manage Libraries → Search "ModbusMaster" → Install by Doc Walker

#### Basic Wiring

```
ES32C14 Terminal    →    RS485 Device
────────────────────────────────────────
A+                  →    A+
B-                  →    B-
GND                 →    GND
```

**Note:** If communication fails, try swapping A+ and B- (most common issue!)

#### Upload Your First Example

1. **Connect** ES32C14 to computer via USB
2. **Open** Arduino IDE
3. **Select Board**: Tools → Board → ESP32 Arduino → ESP32 Dev Module
4. **Select Port**: Tools → Port → (your COM port)
5. **Open Example**: File → Open → `examples/02_BasicRS485_Echo/BasicRS485_Echo.ino`
6. **Upload**: Click Upload button
7. **Test**: LED should blink every 2 seconds

### Examples Overview

#### Example 01: BasicRS485_Official
Original example from eletechsup (http://www.485io.com)
- Simple RS485 send/receive test
- GPIO22 control for TX/RX mode
- Baudrate: 115200

**Use case:** Learning RS485 basics, testing hardware

#### Example 02: BasicRS485_Echo
Enhanced version with diagnostics
- LED indicator (GPIO2) for transmit status
- Auto-send test messages every 2 seconds
- Echo received data back to sender
- Baudrate: 9600 (standard Modbus)

**Use case:** Testing RS485 connection, verifying wiring

#### Example 03: ModbusRTU_Master
Read HT73 temperature/humidity sensor
- Uses ModbusMaster library
- Reads registers via FC03
- LED blink patterns for status
- Includes debugging tips

**Use case:** Reading Modbus RTU sensors

#### Example 04: ModbusRTU_Scanner
Scan for Modbus devices with WiFi web interface
- Scans Slave IDs 1-10 (configurable to 1-247)
- WiFi AP mode: "ES32C14-Scanner"
- Web UI at http://192.168.4.1
- Tests FC03 and FC04 function codes

**Use case:** Finding unknown Modbus device addresses

### Documentation

📘 **[TESTING.md](docs/TESTING.md)** - How to test MODBUS and manufacturer code  
📘 **[PINOUT.md](docs/PINOUT.md)** - ES32C14 pin reference and connections  
📘 **[WIRING.md](docs/WIRING.md)** - Complete RS485 wiring guide  
📘 **[TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)** - Common issues and solutions  
📘 **[Hardware Specs](hardware/README.md)** - Technical specifications

### Important Notes

⚠️ **Serial Monitor Won't Work!**  
The ES32C14 uses UART0 for RS485, which is the same UART used by USB. You cannot use Serial Monitor for debugging. Instead:
- Use WiFi web interface (Example 04)
- Use Serial2 on GPIO16/17 for debug output
- Use LED blink patterns
- See [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) for details

⚠️ **A+ and B- Wiring**  
If communication doesn't work, the first thing to try is **swapping A+ and B- wires**. This fixes 80% of connection issues!

### Common Issues

| Issue | Solution |
|-------|----------|
| No communication | Swap A+ and B- wires |
| Intermittent errors | Add 120Ω termination resistors |
| Cannot debug | Use Serial2 or WiFi (see docs) |
| Wrong readings | Divide by 10 (HT73 values) |
| Upload fails | Hold BOOT button during upload |

See [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) for complete troubleshooting guide.

### Community and Support

- **Issues:** [GitHub Issues](https://github.com/IT-Buster/ESP32C14-RS485-Demo/issues)
- **Discussions:** [GitHub Discussions](https://github.com/IT-Buster/ESP32C14-RS485-Demo/discussions)
- **Original Manufacturer:** [eletechsup](http://www.485io.com)

### Contributing

Contributions are welcome! Please feel free to submit:
- Bug reports
- Feature requests
- Documentation improvements
- New examples
- Translations

### Credits

- **Original BasicRS485 Example:** [eletechsup](http://www.485io.com)
- **ModbusMaster Library:** [Doc Walker (4-20ma)](https://github.com/4-20ma/ModbusMaster)
- **ESP32 Arduino Core:** [Espressif Systems](https://github.com/espressif/arduino-esp32)

### License

This project is licensed under the **MIT License** - see [LICENSE](LICENSE) file for details.

The original BasicRS485 example code is credited to eletechsup (http://www.485io.com).

---

## 🇵🇱 Wersja Polska

### Przegląd

To repozytorium zawiera kompletny zestaw przykładów Arduino i dokumentacji dla **płytki rozszerzeń ES32C14** - płytki opartej na ESP32 z wbudowanym transponderem RS485. Idealna do komunikacji przemysłowej, sieci czujników i aplikacji Modbus RTU.

### Funkcje

✅ **4 Kompletne Przykłady Arduino** - Od podstawowego RS485 do zaawansowanego skanera Modbus  
✅ **Obszerna Dokumentacja** - Wyprowadzenia, okablowanie, rozwiązywanie problemów  
✅ **Przyjazne dla Początkujących** - Szczegółowe komentarze i instrukcje krok po kroku  
✅ **Rzeczywiste Zastosowania** - Odczyt czujnika HT73, skanowanie urządzeń  
✅ **Integracja WiFi** - Monitorowanie i sterowanie przez przeglądarkę  
✅ **Dokumentacja Dwujęzyczna** - Polska i angielska dokumentacja

### Co zawiera projekt

```
ESP32C14-RS485-Demo/
├── examples/
│   ├── 01_BasicRS485_Official/     # Oryginalny przykład eletechsup
│   ├── 02_BasicRS485_Echo/         # Rozszerzony test echo z LED
│   ├── 03_ModbusRTU_Master/        # Odczyt czujnika HT73 przez Modbus
│   └── 04_ModbusRTU_Scanner/       # Skanowanie urządzeń Modbus (WiFi)
├── docs/
│   ├── PINOUT.md                   # Opis wyprowadzeń ES32C14
│   ├── WIRING.md                   # Przewodnik okablowania RS485
│   └── TROUBLESHOOTING.md          # Typowe problemy i rozwiązania
├── hardware/
│   └── README.md                   # Specyfikacja techniczna
└── README.md                       # Ten plik
```

### Szybki Start

#### Wymagania Sprzętowe

1. **Płytka Rozszerzeń ES32C14** (z modułem ESP32)
2. **Kabel USB** (Micro-USB lub USB-C)
3. **Urządzenie RS485** (np. czujnik HT73, druga płytka ES32C14, lub adapter USB-RS485)
4. **Przewody** i/lub kabel RS485
5. **Rezystory 120Ω** (opcjonalnie, dla kabli >10m)

#### Wymagania Programowe

1. **Arduino IDE** 2.0 lub nowsze
2. **Pakiet Płytek ESP32**:
   - Plik → Preferencje → Dodatkowe adresy URL menedżera płytek:
   - `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Narzędzia → Płytka → Menedżer płytek → Szukaj "ESP32" → Instaluj
3. **Biblioteka ModbusMaster** (dla przykładów 3 i 4):
   - Narzędzia → Zarządzaj bibliotekami → Szukaj "ModbusMaster" → Instaluj (autor: Doc Walker)

#### Podstawowe Okablowanie

```
Terminal ES32C14    →    Urządzenie RS485
────────────────────────────────────────────
A+                  →    A+
B-                  →    B-
GND                 →    GND
```

**Uwaga:** Jeśli komunikacja nie działa, spróbuj zamienić A+ i B- miejscami (najczęstszy problem!)

#### Wgraj Pierwszy Przykład

1. **Podłącz** ES32C14 do komputera przez USB
2. **Otwórz** Arduino IDE
3. **Wybierz Płytkę**: Narzędzia → Płytka → ESP32 Arduino → ESP32 Dev Module
4. **Wybierz Port**: Narzędzia → Port → (twój port COM)
5. **Otwórz Przykład**: Plik → Otwórz → `examples/02_BasicRS485_Echo/BasicRS485_Echo.ino`
6. **Wgraj**: Kliknij przycisk Wgraj
7. **Testuj**: LED powinien migać co 2 sekundy

### Przegląd Przykładów

#### Przykład 01: BasicRS485_Official
Oryginalny przykład od eletechsup (http://www.485io.com)
- Prosty test wysyłania/odbierania RS485
- Sterowanie GPIO22 dla trybu TX/RX
- Prędkość: 115200

**Zastosowanie:** Nauka podstaw RS485, testowanie sprzętu

#### Przykład 02: BasicRS485_Echo
Rozszerzona wersja z diagnostyką
- Wskaźnik LED (GPIO2) dla statusu transmisji
- Automatyczne wysyłanie wiadomości testowych co 2 sekundy
- Echo odebranych danych z powrotem
- Prędkość: 9600 (standardowy Modbus)

**Zastosowanie:** Testowanie połączenia RS485, weryfikacja okablowania

#### Przykład 03: ModbusRTU_Master
Odczyt czujnika temperatury/wilgotności HT73
- Używa biblioteki ModbusMaster
- Odczyt rejestrów przez FC03
- Wzory migania LED dla statusu
- Zawiera wskazówki debugowania

**Zastosowanie:** Odczyt czujników Modbus RTU

#### Przykład 04: ModbusRTU_Scanner
Skanowanie urządzeń Modbus z interfejsem WiFi
- Skanuje adresy 1-10 (konfigurowalne do 1-247)
- Tryb AP WiFi: "ES32C14-Scanner"
- Interfejs WWW pod adresem http://192.168.4.1
- Testuje kody funkcji FC03 i FC04

**Zastosowanie:** Znajdowanie nieznanych adresów urządzeń Modbus

### Dokumentacja

📘 **[TESTING.md](docs/TESTING.md)** - Jak testować MODBUS i kod producenta  
📘 **[PINOUT.md](docs/PINOUT.md)** - Opis wyprowadzeń i połączeń ES32C14  
📘 **[WIRING.md](docs/WIRING.md)** - Kompletny przewodnik okablowania RS485  
📘 **[TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)** - Typowe problemy i rozwiązania  
📘 **[Specyfikacja Sprzętu](hardware/README.md)** - Specyfikacja techniczna

### Ważne Uwagi

⚠️ **Monitor Szeregowy Nie Będzie Działać!**  
ES32C14 używa UART0 do RS485, który jest tym samym UART używanym przez USB. Nie możesz używać Monitora Szeregowego do debugowania. Zamiast tego:
- Użyj interfejsu WWW przez WiFi (Przykład 04)
- Użyj Serial2 na GPIO16/17 do wyjścia debugowania
- Użyj wzorów migania LED
- Zobacz [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) po szczegóły

⚠️ **Okablowanie A+ i B-**  
Jeśli komunikacja nie działa, pierwszą rzeczą do wypróbowania jest **zamiana przewodów A+ i B-**. To rozwiązuje 80% problemów z połączeniem!

### Typowe Problemy

| Problem | Rozwiązanie |
|---------|-------------|
| Brak komunikacji | Zamień przewody A+ i B- |
| Sporadyczne błędy | Dodaj rezystory terminujące 120Ω |
| Nie można debugować | Użyj Serial2 lub WiFi (zobacz dokumentację) |
| Złe odczyty | Podziel przez 10 (wartości HT73) |
| Błąd wgrywania | Przytrzymaj przycisk BOOT podczas wgrywania |

Zobacz [TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) po kompletny przewodnik rozwiązywania problemów.

### Społeczność i Wsparcie

- **Problemy:** [GitHub Issues](https://github.com/IT-Buster/ESP32C14-RS485-Demo/issues)
- **Dyskusje:** [GitHub Discussions](https://github.com/IT-Buster/ESP32C14-RS485-Demo/discussions)
- **Oryginalny Producent:** [eletechsup](http://www.485io.com)

### Wkład w Projekt

Wkład jest mile widziany! Prosimy o zgłaszanie:
- Raportów błędów
- Próśb o nowe funkcje
- Ulepszeń dokumentacji
- Nowych przykładów
- Tłumaczeń

### Podziękowania

- **Oryginalny Przykład BasicRS485:** [eletechsup](http://www.485io.com)
- **Biblioteka ModbusMaster:** [Doc Walker (4-20ma)](https://github.com/4-20ma/ModbusMaster)
- **ESP32 Arduino Core:** [Espressif Systems](https://github.com/espressif/arduino-esp32)

### Licencja

Ten projekt jest licencjonowany na podstawie **licencji MIT** - zobacz plik [LICENSE](LICENSE) po szczegóły.

Oryginalny kod przykładu BasicRS485 pochodzi od eletechsup (http://www.485io.com).

---

**Made with ❤️ for the ESP32 and RS485/Modbus community**