# Day 20 — Ultrasonic Time-of-Flight Proximity Alarm

## 📌 Overview
An automated proximity detection system using an HC-SR04 ultrasonic sensor and an active piezo buzzer. The microcontroller calculates object distance using time-of-flight acoustic pulse reflection, triggering an audible alarm on Pin 8 when an intrusion is detected within 30 cm.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x HC-SR04 Ultrasonic Distance Sensor
* 1x Active Buzzer
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **HC-SR04 VCC** | 5V Power Rail |
| **HC-SR04 GND** | Arduino GND Pin |
| **HC-SR04 Trig** | Digital Pin 2 |
| **HC-SR04 Echo** | Digital Pin 3 |
| **Active Buzzer (+)** | Digital Pin 8 |
| **Active Buzzer (-)** | Arduino GND Pin |

---

## ⚙️ Control Architecture & Physics Logic
1. **Ultrasonic Pulse Generation:** Digital Pin 2 issues a 10µs HIGH pulse to trigger the HC-SR04 transducer array, emitting a 40kHz ultrasonic burst.
2. **Time-of-Flight Calculation:** The `pulseIn()` function reads the duration (in microseconds) that Digital Pin 3 remains HIGH. Distance is calculated using the speed of sound constant ($0.034 \text{ cm/\mu s}$):
   $$\text{Distance (cm)} = \frac{\text{duration} \times 0.034}{2}$$
   *(Divided by 2 to account for the two-way wave travel path to the target and back).*
3. **Actuation & Communications Stability:** If the measured distance falls between 1 cm and 30 cm, Pin 8 drives the buzzer HIGH. A 2-second bootloader startup delay and 300ms loop pacing are configured to prevent UART serial bus flooding during upload cycles.