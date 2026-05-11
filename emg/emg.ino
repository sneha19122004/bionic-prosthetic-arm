#include <ESP32Servo.h>

Servo servo1; 
Servo servo2;

void setup() {
  servo1.attach(12);
  servo2.attach(13);
}

void loop() {
  for (int pos = 0; pos <= 180; pos += 1) {
    servo1.write(pos);
    servo2.write(180 - pos);
    delay(15);
  }
}