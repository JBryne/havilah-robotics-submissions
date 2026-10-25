# Day 22 — Dual-Mode Motor Speed Controller & Thermal Actuation System

## 📌 Overview
A hybrid electromechanical control system featuring dual-mode DC motor PWM driving alongside independent AC/DC relay switching. The controller processes debounced digital inputs on Pins 2 and 3 to toggle system states, enabling either manual speed adjustment via a potentiometer or linear thermal speed scaling using a TMP36 temperature sensor.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x DC Motor Driver / Transistor Circuit
* 1x 5V Relay Module
* 1x TMP36 Analog Temperature Sensor
* 1x 10kΩ Potentiometer
* 2x Tactile Pushbuttons
* 1x Standard LED (Status Indicator)
* 1x 220Ω Resistor
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **Pushbutton 1 (Motor Mode)** | Digital Pin 2 (Internal `INPUT_PULLUP`) |
| **Pushbutton 2 (Relay Drive)** | Digital Pin 3 (Internal `INPUT_PULLUP`) |
| **Relay Signal Line** | Digital Pin 4 |
| **Motor Drive Line (PWM)** | Digital Pin 5 |
| **Status LED Anode** | 220Ω Resistor → Digital Pin 13 |
| **Potentiometer Signal** | Analog Pin A0 |
| **TMP36 Signal Line** | Analog Pin A1 |
| **Power / Ground Rails** | 5V Rail / GND Rail |

---

## ⚙️ System Architecture & Logic
1. **Debounced Mode Toggling:** Pins 2 and 3 use `INPUT_PULLUP` with a 50ms software debounce timer. Pushbutton 2 directly toggles the relay state on Pin 4, while Pushbutton 1 switches the motor between **`MANUAL`** and **`AUTO`** modes.
2. **Dual-Mode Speed Calculation:**
   * **`MANUAL` Mode:** Maps `analogRead(A0)` from the potentiometer ($0\text{--}1023$) to an 8-bit PWM duty cycle ($0\text{--}255$).
   * **`AUTO` Mode:** Reads voltage from the TMP36 sensor on A1 where $\text{Temp}(^\circ\text{C}) = (V - 0.5) \times 100$. PWM output scales linearly from $0\%$ at $25.0^\circ\text{C}$ to $100\%$ ($255\text{ PWM}$) at $35.0^\circ\text{C}$.
3. **Actuation & Telemetry:** Pin 5 outputs the evaluated PWM signal to drive motor speed, while Pin 13 illuminates an indicator LED whenever PWM output is active ($> 0$). System metrics (temperature, relay state, active mode, and speed percentage) stream over Serial every 500ms.