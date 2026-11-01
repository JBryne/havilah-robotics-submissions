# Day 24 — Automated Smart Door Latch & Manual Lock Override

## 📌 Overview
An integrated access control system combining ultrasonic proximity detection, servo-driven latch actuation, dual visual indicators, acoustic feedback, and a manual lock override. An HC-SR04 sensor monitors incoming targets within 15 cm to automatically actuate an SG90 servo motor while a debounced pushbutton provides manual lock override security.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x HC-SR04 Ultrasonic Distance Sensor
* 1x SG90 Servo Motor
* 1x Tactile Pushbutton
* 1x Red LED (Locked Indicator)
* 1x Green LED (Unlocked Indicator)
* 1x Active/Passive Buzzer
* 2x 220Ω Resistors
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **HC-SR04 Trig** | Digital Pin 2 |
| **HC-SR04 Echo** | Digital Pin 3 |
| **Manual Button** | Digital Pin 4 (Internal `INPUT_PULLUP`) |
| **Red LED Anode** | 220Ω Resistor → Digital Pin 5 |
| **Green LED Anode** | 220Ω Resistor → Digital Pin 6 |
| **Buzzer (+)** | Digital Pin 7 |
| **Servo Signal (PWM)** | Digital Pin 9 |
| **Power Rails (VCC / GND)** | 5V Rail / GND Rail |

---

## ⚙️ System Architecture & Logic
1. **Distance Calculation & Automatic Unlocking:** The HC-SR04 emits a 10µs trigger pulse and measures echo reflection duration. Distance is calculated via $d = \text{duration} \times 0.0343 / 2$. When a target is detected within $15\text{ cm}$ (and override mode is inactive), the servo rotates from $0^\circ$ (Locked) to $90^\circ$ (Unlocked), the Green LED on Pin 6 illuminates, and a 1000Hz chirp plays on Pin 7.
2. **Debounced Manual Lock Override:** A software-debounced pushbutton on Pin 4 (`INPUT_PULLUP`, 50ms interval) toggles `isOverrideLocked`. When override is enabled, the control loop ignores proximity triggers, forcing the door to remain securely locked at $0^\circ$ with the Red LED on Pin 5 active regardless of distance readings.