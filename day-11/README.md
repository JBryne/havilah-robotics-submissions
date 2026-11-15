# Day 26 — Standalone Ultrasonic Rangefinder with 16x2 LCD Display

## 📌 Overview
A self-contained ultrasonic distance measurement unit that outputs live range telemetry directly to a 16x2 character LCD without requiring a Serial Monitor connection. Utilizing an HC-SR04 sensor and a 4-bit parallel LCD interface, the system computes target distance to one decimal place and handles out-of-range conditions via timeout detection.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x HC-SR04 Ultrasonic Distance Sensor
* 1x 16x2 Character LCD Display
* 1x 10kΩ Potentiometer (LCD Contrast Adjustment)
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **LCD RS (Pin 4)** | Digital Pin 12 |
| **LCD Enable (Pin 6)** | Digital Pin 11 |
| **LCD Data Lines D4, D5, D6, D7** | Digital Pins 5, 4, 3, 2 |
| **HC-SR04 Trig** | Digital Pin 9 |
| **HC-SR04 Echo** | Digital Pin 10 |
| **Power Rails (VCC / GND)** | 5V Rail / GND Rail |

---

## ⚙️ System Architecture & Logic
1. **4-Bit Parallel LCD Interface:** Configured via `LiquidCrystal(12, 11, 5, 4, 3, 2)` to drive the display using four data lines, preserving GPIO pins for sensor interfacing.
2. **Time-of-Flight Measurement & Timeout Handling:** Pin 9 emits a 10µs ultrasonic pulse while `pulseIn(echoPin, HIGH, 30000)` listens on Pin 10 with a 30ms timeout ceiling.
   * **Valid Echo:** Converts round-trip travel duration into centimeters ($d = \text{echoTime} \times 0.0343 / 2.0$) and renders the reading to `0.1 cm` precision every 500ms.
   * **Out-of-Range (`echoTime == 0`):** Prevents blocking delays when no echo returns by clearing the screen and printing `"NO ECHO"` on row 1.