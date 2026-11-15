# Day 27 — Digital Thermostat & Hysteresis Thermal Alarm

## 📌 Overview
A standalone thermal monitoring and alarm system combining a DHT11 temperature/humidity sensor with a 16x2 parallel LCD, visual LED indicator, and acoustic buzzer. The control logic uses dual-threshold hysteresis to prevent rapid alarm chatter near the temperature boundary while providing on-screen sensor fault diagnostics.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x 16x2 Character LCD Display
* 1x 10kΩ Potentiometer (LCD Contrast)
* 1x DHT11 Temperature & Humidity Sensor
* 1x Red LED
* 1x 220Ω Resistor
* 1x Buzzer
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **LCD RS (Pin 4)** | Digital Pin 12 |
| **LCD Enable (Pin 6)** | Digital Pin 11 |
| **LCD Data Lines D4, D5, D6, D7** | Digital Pins 5, 4, 3, 2 |
| **Red LED Anode** | 220Ω Resistor → Digital Pin 7 |
| **Buzzer (+)** | Digital Pin 8 |
| **DHT11 Signal ('S')** | Digital Pin 9 |
| **Power Rails (VCC / GND)** | 5V Rail / GND Rail |

---

## ⚙️ System Architecture & Logic
1. **Hysteresis Alarm Control:** Rather than switching at a single temperature point, the system uses a 5.0°C deadband window. When ambient temperature rises above `30.0°C` (`tempLimit`), `alarmOn` latches `true`, energizing the Red LED (Pin 7) and buzzer (Pin 8). The alarm remains active until the temperature cools down to `25.0°C` or below (`tempReset`), preventing rapid on/off flickering.
2. **4-Bit LCD Interface & Fault Detection:** The 16x2 LCD operates in 4-bit parallel mode (`LiquidCrystal(12, 11, 5, 4, 3, 2)`). If the DHT11 returns invalid `NaN` readings due to loose wiring or timeout, the loop halts actuation and prints `"ERROR / CHECK ERROR"` directly on the display.