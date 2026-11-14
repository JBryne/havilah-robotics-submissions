# Day 25 — Dual Heat & Intruder Alarm System (DHT11 + LDR)

## 📌 Overview
A multi-sensor security and environmental hazard monitoring system built around a 5-state non-blocking Finite State Machine (FSM). By combining a DHT11 digital temperature sensor with a photoresistor (LDR) voltage divider, the system independently detects thermal hazards and shadow intrusions, outputting distinct acoustic siren frequencies and LED status alerts.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x DHT11 Temperature & Humidity Sensor
* 1x Light Dependent Resistor (LDR)
* 1x 10kΩ Resistor (LDR Voltage Divider)
* 1x Active/Passive Buzzer
* 1x Tactile Pushbutton
* 1x Green LED (Armed Status)
* 1x Red LED (Hazard Alert)
* 2x 220Ω Resistors
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **Pushbutton Terminal 1** | Digital Pin 2 (Internal `INPUT_PULLUP`) |
| **Pushbutton Terminal 2** | Arduino GND Pin |
| **DHT11 Signal ('S')** | Digital Pin 3 |
| **Buzzer (+)** | Digital Pin 7 |
| **Green LED Anode** | 220Ω Resistor → Digital Pin 8 |
| **Red LED Anode** | 220Ω Resistor → Digital Pin 9 |
| **LDR Divider Junction** | Analog Pin A0 (with 10kΩ pull-down to GND) |
| **Power Rails (VCC / GND)** | 5V Rail / GND Rail |

---

## ⚙️ System Architecture & Logic
1. **5-State Machine (`SystemState`):**
   * **`DISARMED`:** Standby mode; all LEDs remain off and the buzzer is silent.
   * **`ARMED`:** Illuminates the Green LED (Pin 8) and samples the DHT11 and LDR every 2000ms using `millis()`.
   * **`ALARM_HEAT`:** Triggered when temperature exceeds `40.0°C`. Engages the Red LED (Pin 9) and alternates Pin 7 between `1000Hz` and `1500Hz` every 120ms.
   * **`ALARM_INTRUDER`:** Triggered when ambient light drops below `300 ADC`. Engages the Red LED and alternates Pin 7 between `500Hz` and `800Hz` every 120ms.
   * **`ALARM_DUAL`:** Triggered when both heat and intruder thresholds are breached simultaneously, alternating Pin 7 between `600Hz` and `1800Hz` every 120ms.
2. **Non-Blocking Execution & Override Reset:** Software debouncing (`50ms`) on Pin 2 toggles between `DISARMED` and `ARMED`, and immediately resets the system back to `DISARMED` from any active alarm state without blocking sensor or siren timers.