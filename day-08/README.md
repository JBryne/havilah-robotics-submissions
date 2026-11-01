# Day 23 — LCD 16x2 Interactive Weather Station & Visual Telemetry

## 📌 Overview
An integrated environmental monitoring unit utilizing an HD44780-driven 16x2 character LCD operating in 4-bit parallel mode. The system processes temperature and humidity data from a DHT11 sensor, displaying real-time readings while supporting dynamic unit conversion (°C to °F) via a debounced tactile switch and visual over-temperature warnings.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x 16x2 Character LCD Display
* 1x 10kΩ Potentiometer (LCD Contrast Tuning)
* 1x DHT11 Temperature & Humidity Sensor
* 1x Tactile Pushbutton
* 1x Red LED (Over-Temp Indicator)
* 1x 220Ω Resistor
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **LCD RS (Pin 4)** | Digital Pin 12 |
| **LCD Enable (Pin 6)** | Digital Pin 11 |
| **LCD Data Lines D4, D5, D6, D7** | Digital Pins 5, 4, 3, 2 |
| **LCD V0 (Pin 3)** | Potentiometer Wiper Terminal |
| **DHT11 Signal Line** | Digital Pin 7 |
| **Pushbutton Terminal 1** | Digital Pin 8 (Internal `INPUT_PULLUP`) |
| **Red LED Anode** | 220Ω Resistor → Digital Pin 6 |
| **Power Rails (VCC / GND)** | 5V Rail / GND Rail |

---

## ⚙️ Logic & Interface Control
1. **4-Bit Parallel LCD Bus:** Driving the display via `LiquidCrystal(12, 11, 5, 4, 3, 2)` utilizes 4 data lines instead of 8, saving microcontroller GPIO pins while maintaining full character control. The potentiometer adjusts the analog bias voltage at pin V0 to balance pixel contrast.
2. **Interactive Unit Toggle & Over-Temp Alert:** A debounced button on Pin 8 toggles the display mode variable (`displayFahrenheit`), dynamically converting values using $T_{^\circ\text{F}} = (T_{^\circ\text{C}} \times 1.8) + 32$. If the DHT11 reads above $30.0^\circ\text{C}$, Pin 6 drives the red LED `HIGH` and appends `"Too Hot!"` to row 0.