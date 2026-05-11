/*
 * EMG Servo Control — Bionic Prosthetic Arm
 *
 * Reads EMG (electromyography) muscle signal from an analog sensor and drives
 * four servo motors to open or close the prosthetic hand accordingly.
 *
 * Hardware:
 *   - Arduino (Uno / Nano or compatible)
 *   - EMG sensor output  → A0
 *   - Servo 1 (index)    → D9
 *   - Servo 2 (middle)   → D10
 *   - Servo 3 (ring)     → D11
 *   - Servo 4 (pinky)    → D6
 *
 * EMG thresholds (tune to your sensor):
 *   ≤ 450          → hand open  (relaxed muscle)
 *   > 450          → hand closed (muscle contracted)
 */

#include <Servo.h>

// ── Pin assignments ──────────────────────────────────────────────
const int EMG_PIN    = A0;
const int SERVO_PIN1 =  9;  // Index finger
const int SERVO_PIN2 = 10;  // Middle finger
const int SERVO_PIN3 = 11;  // Ring finger
const int SERVO_PIN4 =  6;  // Pinky finger

// ── Threshold ────────────────────────────────────────────────────
const int EMG_THRESHOLD = 450;

// ── Servo objects ────────────────────────────────────────────────
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// ────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  servo1.attach(SERVO_PIN1);
  servo2.attach(SERVO_PIN2);
  servo3.attach(SERVO_PIN3);
  servo4.attach(SERVO_PIN4);

  // Start in open (relaxed) position
  openHand();
}

// ────────────────────────────────────────────────────────────────
void loop() {
  int emgValue = analogRead(EMG_PIN);
  Serial.print("EMG Value: ");
  Serial.println(emgValue);

  if (emgValue <= EMG_THRESHOLD) {
    openHand();
  } else {
    closeHand();
  }

  delay(100);  // ~10 Hz polling rate
}

// ── Helper functions ─────────────────────────────────────────────

// Fingers extended — hand open
void openHand() {
  servo1.write(0);
  servo2.write(180);
  servo3.write(0);
  servo4.write(0);
}

// Fingers curled — hand closed / grip
void closeHand() {
  servo1.write(180);
  servo2.write(0);
  servo3.write(90);
  servo4.write(180);
}
