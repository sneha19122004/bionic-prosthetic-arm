# Bionic Prosthetic Arm

An Arduino/ESP32-based bionic prosthetic arm that responds to EMG (electromyography) muscle signals to open and close a 3D-printed hand, with integrated temperature sensing for safety feedback.

---

## Repository Structure

```
bionic-prosthetic-arm/
├── src/
│   ├── emg_servo_control/
│   │   └── emg_servo_control.ino   # EMG → servo motor control
│   └── temperature_sensor/
│       └── temperature_sensor.ino  # Thermocouple + OLED display
├── models/
│   ├── palm.stl
│   ├── wrist.stl
│   ├── clip.stl
│   ├── Finger_Index.stl
│   ├── Finger_Middle.stl
│   ├── Finger_Ring.stl
│   ├── Finger_Pinky.stl
│   └── Finger_Thumb.stl
└── README.md
```

---

## Modules

### 1. EMG Servo Control (`src/emg_servo_control/`)

Reads analog EMG sensor output and drives four servo motors to actuate the prosthetic fingers.

| EMG Signal | Action |
|---|---|
| ≤ 450 (relaxed) | Hand opens |
| > 450 (contracted) | Hand closes |

**Hardware:**
- Arduino Uno / Nano (or compatible)
- EMG sensor → A0
- 4× servo motors → D6, D9, D10, D11

**Libraries required:**
- `Servo.h` (built-in)

---

### 2. Temperature Sensor (`src/temperature_sensor/`)

Reads contact temperature via a MAX6675 K-type thermocouple and displays a 3-sample rolling average on an SSD1306 OLED. Provides burn/cold alerts.

| Temperature | Alert |
|---|---|
| > 45 °C | Danger — Burn Risk |
| < 10 °C | Warning — Too Cold |

**Hardware:**
- ESP32
- MAX6675 thermocouple module → SCK=18, CS=5, SO=19
- SSD1306 OLED 128×64 → I²C

**Libraries required:**
- `Adafruit GFX Library`
- `Adafruit SSD1306`
- `MAX6675 library` (by Adafruit or compatible)

Install via Arduino Library Manager or `platformio lib install`.

---

## 3D Printed Models (`models/`)

Print all `.stl` files in PLA or PETG. Recommended settings: 20% infill, 0.2 mm layer height, supports enabled for the palm and wrist.

| File | Part |
|---|---|
| `palm.stl` | Main palm body |
| `wrist.stl` | Wrist joint |
| `clip.stl` | Tendon/cable clip |
| `Finger_*.stl` | Individual finger segments |

---
## Final Working Model

<p align="center">
<a href="https://github.com/sneha19122004/bionic-prosthetic-arm/blob/master/final-outcome.mp4">
<img src="https://img.shields.io/badge/%20Watch%20Demo-Prosthetic%20palm-green?style=for-the-badge" alt="Watch Working Robot Demo"/>
</a>
</p>

> Download / view working demo → [`/working-video`](https://github.com/sneha19122004/bionic-prosthetic-arm/blob/master/final-outcome.mp4)

---
## Getting Started

1. **Clone the repo**
   ```bash
   git clone https://github.com/your-username/bionic-prosthetic-arm.git
   cd bionic-prosthetic-arm
   ```

2. **Open a sketch** in the Arduino IDE (File → Open → select the `.ino` file).

3. **Install dependencies** via Tools → Manage Libraries.

4. **Select your board** under Tools → Board, then upload.

5. **Print the models** and assemble per the wiring diagram.
---

## License

MIT License — see [LICENSE](LICENSE) for details.
