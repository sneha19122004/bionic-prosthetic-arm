/*
 * Temperature Sensing — Bionic Prosthetic Arm
 *
 * Reads skin/contact temperature via a MAX6675 K-type thermocouple and
 * displays a rolling 3-sample average on an SSD1306 OLED. Alerts the
 * user if temperature is dangerously high (burn risk) or too low.
 *
 * Hardware (ESP32):
 *   - SSD1306 OLED 128×64  → I²C (SDA/SCL)
 *   - MAX6675 thermocouple → SCK=18, CS=5, SO=19
 *
 * Alert thresholds:
 *   > 45 °C  →  Danger: Burn Risk
 *   < 10 °C  →  Warning: Too Cold
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <max6675.h>
#include <SPI.h>

// ── Display config ───────────────────────────────────────────────
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ── MAX6675 pin assignments ───────────────────────────────────────
#define PIN_SCK  18
#define PIN_CS    5
#define PIN_SO   19
MAX6675 thermocouple(PIN_SCK, PIN_CS, PIN_SO);

// ── Averaging ────────────────────────────────────────────────────
const int NUM_READINGS = 3;

// ── Alert thresholds (°C) ────────────────────────────────────────
const float TEMP_HIGH = 45.0;
const float TEMP_LOW  = 10.0;

// ────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("ERROR: OLED not found — check wiring.");
    while (1);  // Halt
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10);
  display.println("Initializing...");
  display.display();
  delay(2000);

  Serial.println("MAX6675 Temperature Sensor Ready");
}

// ────────────────────────────────────────────────────────────────
void loop() {
  float total = 0.0;

  for (int i = 0; i < NUM_READINGS; i++) {
    float tempC = thermocouple.readCelsius();

    // MAX6675 returns a value below 0 when no thermocouple is connected
    if (tempC < 0) {
      Serial.println("ERROR: No thermocouple detected.");
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(10, 20);
      display.println("No Sensor!");
      display.display();
      delay(2000);
      return;
    }

    Serial.print("Reading ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(tempC, 1);
    Serial.println(" C");

    total += tempC;
    delay(1000);  // 1-second gap between readings (MAX6675 minimum)
  }

  float avgTemp = total / NUM_READINGS;
  Serial.print("Average: ");
  Serial.print(avgTemp, 1);
  Serial.println(" C");

  // ── Render to OLED ─────────────────────────────────────────────
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("Avg Temp:");

  display.setCursor(30, 35);
  display.print(avgTemp, 1);
  display.print(" C");

  // Alert banner (bottom row)
  display.setTextSize(1);
  display.setCursor(10, 55);

  if (avgTemp > TEMP_HIGH) {
    display.print("! Danger: Burn Risk!");
    Serial.println("ALERT: High temperature — burn risk!");
  } else if (avgTemp < TEMP_LOW) {
    display.print("* Warning: Too Cold!");
    Serial.println("ALERT: Very low temperature.");
  }

  display.display();
  delay(5000);  // Refresh every 5 seconds
}
