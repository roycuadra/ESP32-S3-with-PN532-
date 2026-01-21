#include <Wire.h>
#include <Adafruit_PN532.h>
#include <Adafruit_NeoPixel.h>

// ===== ESP32-S3 I2C PINS =====
#define I2C_SDA 5 
#define I2C_SCL 6

// ===== RELAY =====
#define RELAY_PIN 7   // 1-channel relay on GPIO 7

// ===== PN532 I2C ONLY =====
Adafruit_PN532 nfc(-1, -1);

// ===== RGB LED =====
#define RGB_PIN     48
#define NUM_PIXELS  1
Adafruit_NeoPixel rgb_led(NUM_PIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);

// ===== AUTHORIZED UID =====
uint8_t allowedUID[] = { 0x53, 0x3E, 0xF9, 0x03 };
const uint8_t uidLength = 4;

void setup() {
  Serial.begin(115200);
  delay(300);

  Serial.println("=== OFFLINE RFID ACCESS (PN532 I2C ONLY) ===");

  // I2C init
  Wire.begin(I2C_SDA, I2C_SCL);

  // Relay init
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);   // Relay OFF at boot

  // RGB init
  rgb_led.begin();
  rgb_led.setBrightness(100);
  rgb_led.clear();
  rgb_led.show();

  // PN532 init
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 not detected!");
    while (1);
  }

  nfc.SAMConfig();
  Serial.println("PN532 ready. Scan card.");
}

void loop() {
  uint8_t uid[7];
  uint8_t uidLen;

  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLen)) {

    Serial.print("UID: ");
    for (uint8_t i = 0; i < uidLen; i++) {
      if (uid[i] < 0x10) Serial.print("0");
      Serial.print(uid[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    if (isAuthorized(uid, uidLen)) {
      accessGranted();
    } else {
      accessDenied();
    }

    delay(1500);               // debounce
    digitalWrite(RELAY_PIN, LOW); // Relay OFF after delay
    rgb_led.clear();
    rgb_led.show();
  }
}

bool isAuthorized(uint8_t *uid, uint8_t uidLen) {
  if (uidLen != uidLength) return false;

  for (uint8_t i = 0; i < uidLength; i++) {
    if (uid[i] != allowedUID[i]) return false;
  }
  return true;
}

void accessGranted() {
  Serial.println("ACCESS GRANTED");
  rgb_led.setPixelColor(0, rgb_led.Color(0, 255, 0)); // Green
  rgb_led.show();

  digitalWrite(RELAY_PIN, HIGH); // Relay ON
}

void accessDenied() {
  Serial.println("ACCESS DENIED");
  rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0)); // Red
  rgb_led.show();
}
