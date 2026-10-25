# Day 21 — Finite State Machine (FSM) Temperature Alarm System

## 📌 Overview
An multi-state thermal monitoring system driven by a DHT11 digital sensor, dual visual indicators, an active acoustic siren, and a software-debounced arm/disarm pushbutton. Built on a non-blocking Finite State Machine (FSM) architecture, the controller continuously evaluates ambient room temperature against a 30.0°C threshold without relying on blocking delay calls.

---

## 🛠️ Components
* 1x Arduino Board
* 1x Breadboard
* 1x DHT11 Temperature & Humidity Sensor
* 1x Tactile Pushbutton
* 1x Active/Passive Buzzer
* 1x Green LED (Armed Indicator)
* 1x Red LED (Alarm Indicator)
* 2x 220Ω Resistors
* Jumper Wires

---

## 🔌 Circuit Wiring

| Component / Pin | Connected To |
| :--- | :--- |
| **DHT11 Signal ('S')** | Digital Pin 3 |
| **DHT11 VCC / GND** | 5V Rail / GND Rail |
| **Pushbutton Terminal 1** | Digital Pin 2 (Internal `INPUT_PULLUP`) |
| **Pushbutton Terminal 2** | Arduino GND Pin |
| **Active Buzzer (+)** | Digital Pin 7 |
| **Green LED Anode** | 220Ω Resistor → Digital Pin 8 |
| **Red LED Anode** | 220Ω Resistor → Digital Pin 9 |
| **LED Cathodes / Buzzer (-)** | Arduino GND Rail |

---

## ⚙️ System Architecture & Logic
1. **Finite State Machine Architecture:** The system manages state transitions across three distinct operational modes via an `enum`:
   * **`DISARMED`:** Sensor outputs logged to Serial; all actuators and LEDs remain inactive.
   * **`ARMED`:** Green LED illuminated on Pin 8. Evaluates DHT11 readings every 2000ms.
   * **`ALARM`:** Triggered when `tempC > 30.0°C`. Red LED illuminates on Pin 9 while a non-blocking dual-frequency siren (alternating 1200Hz and 800Hz tones every 150ms) sounds on Pin 7.
2. **Software Debouncing & Non-Blocking Loops:** Pin 2 utilizes internal pull-up resistors with a 50ms software debouncing threshold to prevent edge-bounce state triggering. Sensor polling and audio frequency modulation operate purely on `millis()` timing counters, preserving CPU cycle availability for real-time state processing.