# Day 17 — Photoresistor (LDR) Voltage Divider & Automatic Light Switch

## 📌 Overview
An automated ambient light detection system built using a Light Dependent Resistor (LDR) configured in a voltage divider circuit. The Arduino reads continuous analog signals via Pin A0, converting ambient light levels into a 10-bit value (0–1023). When light levels drop below a calibrated threshold of 500, the system automatically actuates the Pin 8 indicator LED while streaming live telemetry over Serial.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x Light Dependent Resistor (LDR)
* 1x 10kΩ Fixed Resistor (Pull-down / Voltage Divider)
* 1x 220Ω Resistor
* 1x Standard LED
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **LDR Leg 1** | 5V Power Rail |
| **LDR Leg 2 / Divider Node** | Analog Pin A0 & 10kΩ Resistor |
| **10kΩ Resistor Terminal 2** | Arduino GND Pin |
| **Arduino Pin 8** | 220Ω Resistor → LED Anode |
| **LED Cathode** | Arduino GND Pin |

---

## ⚙️ Logic & Implementation
1. **Voltage Divider Principle:** Because the LDR changes resistance based on light exposure, placing it in series with a fixed 10kΩ resistor creates a variable voltage divider. In bright light, LDR resistance is low (higher voltage at A0). In darkness, LDR resistance increases significantly, pulling the voltage at A0 lower toward GND.
2. **Threshold Logic & Telemetry:** `analogRead(A0)` samples the voltage divider node every 500ms. An `if-else` condition evaluates if the reading falls below 500. If met, Pin 8 goes `HIGH` to turn on the LED; otherwise, it remains `LOW`. Live values stream continuously at 9600 baud for threshold tuning.