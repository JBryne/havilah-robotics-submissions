# Day 01 — LED Circuit & Serial Debugging

## 📌 Overview
A foundational breadboard circuit using Arduino Pin 13 to power an LED through a 220Ω current-limiting resistor. The project demonstrates modifying pulse delays (100ms vs 2000ms) and tracking real-time hardware states via the Serial Monitor at 9600 baud.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x Standard LED
* 1x 220Ω Resistor
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **Arduino Pin 13** | 220Ω Resistor |
| **220Ω Resistor** | LED Anode (Long leg) |
| **LED Cathode (Short leg)** | Arduino GND Pin |

---

## ⚙️ Key Observations & Logic
1. **Serial Logging:** `Serial.begin(9600)` initializes communication. The sketch outputs `"LED ON"` and `"LED OFF"` to the Serial Monitor in sync with `digitalWrite(13, HIGH)` and `LOW`.
2. **Timing Control:** 
   * **100ms Delay:** Rapid blinking effect demonstrating fast switching frequency.
   * **2000ms Delay:** Slow 2-second pulse cycle illustrating controlled delay loops.
