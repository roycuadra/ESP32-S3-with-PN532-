# ESP32-S3 Simple RFID Access Control (PN532 I2C)

This project is an **Simple RFID access control system** using an **ESP32-S3**, **PN532 NFC/RFID module (I2C mode)**, a **relay**, and a **NeoPixel RGB LED**.

When an authorized RFID/NFC card is scanned, the relay is activated and the RGB LED turns green. Unauthorized cards trigger a red LED and no relay activation.

No internet connection or external database is required.

---

## Features

- Offline RFID authentication
- PN532 operating strictly in **I2C mode**
- UID-based access control
- Relay output for door lock or load control
- NeoPixel RGB LED status indicator
- Serial monitor debugging output

---

## System Behavior

1. ESP32-S3 initializes I2C on GPIO 5 (SDA) and GPIO 6 (SCL)
2. PN532 waits for an RFID/NFC card
3. Card UID is read and printed to Serial Monitor
4. UID is compared against a predefined authorized UID
5. If UID matches:
   - Relay turns ON
   - RGB LED turns GREEN
6. If UID does not match:
   - Relay remains OFF
   - RGB LED turns RED
7. After a short delay, the system resets and waits for the next scan

---

## Hardware Requirements

- ESP32-S3 DevKit
- PN532 NFC/RFID module (I2C mode)
- 1-channel relay module
- NeoPixel RGB LED (WS2812 or compatible)
- Jumper wires
- External power supply for relay/lock (recommended)

---

## Pin Configuration

### I2C Bus

| ESP32-S3 Pin | Function |
|-------------|----------|
| GPIO 5 | SDA |
| GPIO 6 | SCL |

### Other Connections

| Component | ESP32-S3 Pin |
|---------|--------------|
| Relay IN | GPIO 7 |
| NeoPixel Data | GPIO 48 |

---

## PN532 Setup

- PN532 **must be configured to I2C mode**
- Use onboard DIP switches or solder jumpers (depends on module version)
- UART and SPI are not used

---

## Required Libraries

Install the following via Arduino Library Manager:

- Adafruit PN532
- Adafruit NeoPixel
- Wire (built-in)

---

## Authorized Card UID

The authorized UID is hardcoded in the sketch:

```cpp
uint8_t allowedUID[] = { 0x53, 0x3E, 0xF9, 0x03 };
