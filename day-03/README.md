# Day 18 — Sensor-Driven Actuation & Acoustic Alarm System

## 📌 Overview
An integrated sensor-actuator control system that processes analog ambient light signals via a Photoresistor (LDR) to trigger physical movement and acoustic feedback. When light intensity drops below a defined threshold, the system drives an SG90 micro-servo to 90° and pulses an active buzzer while streaming continuous ADC telemetry over 9600 baud Serial.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x Light Dependent Resistor (LDR)
* 1x 10kΩ Fixed Resistor (Voltage Divider)
* 1x Active Buzzer
* 1x SG90 Servo Motor
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **LDR Leg 1** | 5V Power Rail |
| **LDR Leg 2 / Divider Node** | Analog Pin A0 & 10kΩ Resistor |
| **10kΩ Resistor Terminal 2** | Arduino GND Pin |
| **Active Buzzer (+)** | Digital Pin 8 |
| **Active Buzzer (-)** | Arduino GND Pin |
| **Servo Signal (PWM)** | Digital Pin 9 |
| **Servo Power / Ground** | 5V Rail / GND Rail |

---

## ⚙️ Key Logic & Implementation
1. **Analog Telemetry & ADC Mapping:** The 10kΩ resistor and LDR form a voltage divider tapped at Analog Pin A0. `analogRead(A0)` converts the incoming voltage into a 10-bit digital value (0–1023), streamed live to the Serial Monitor every loop.
2. **Synchronized Output Execution:** When `ldrValue` falls below the `500` threshold (darkness condition), the control loop actuates `myServo.write(90)` and sets Pin 8 `HIGH`. A 500ms delay holds the state before resetting the buzzer to `LOW` and homing the servo back to 0°.