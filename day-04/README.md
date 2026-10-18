# Day 19 — Smart Systems Architecture & Hysteresis Control

## 📌 Overview
An automated light-control system built around the fundamental **Sense → Think → Act** embedded design pattern. To prevent high-frequency output toggling (chatter) caused by signal noise near the threshold boundary, the system uses dual-threshold hysteresis (deadband control) to manage the Pin 13 LED based on photoresistor readings from pin A0.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x Light Dependent Resistor (LDR)
* 1x 10kΩ Fixed Resistor (Voltage Divider)
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
| **Arduino Pin 13** | 220Ω Resistor → LED Anode |
| **LED Cathode** | Arduino GND Pin |

---

## ⚙️ Control Architecture & Hysteresis Logic
1. **Sense (Input):** The LDR voltage divider feeds pin A0, mapping ambient light levels into a 10-bit ADC integer value (0–1023).
2. **Think (State Machine & Hysteresis):** A single threshold causes output chatter when ambient light hovers right at the decision boundary. By configuring calibrated thresholds—turning `ON` below **120** and `OFF` above **200**—the code establishes an 80-unit deadband gap where the system maintains its previous state (`ledState`), isolating the actuator from signal noise.
3. **Act (Output & Telemetry):** Pin 13 drives the indicator LED based on the evaluated state machine output while streaming live `LDR Reading` and `LED State` telemetry over 9600 baud Serial at 1000ms intervals.