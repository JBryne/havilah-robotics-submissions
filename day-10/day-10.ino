/*
 * Day 25: Dual Heat & Intruder Alarm System (DHT11 + LDR)
 * 
 * Hardware Pin Mapping:
 *   - Pushbutton:      Pin 2 (INPUT_PULLUP to GND)
 *   - DHT11 Data (S):  Pin 3
 *   - Buzzer (+):      Pin 7
 *   - Green LED:       Pin 8 (via 220Ω resistor to GND)
 *   - Red LED:         Pin 9 (via 220Ω resistor to GND)
 *   - LDR Divider:     Pin A0 (Divider junction with 10kΩ resistor)
 */

#include <DHT.h>

#define DHTPIN 3
#define DHTTYPE DHT11

// Pin Definitions
const int BUTTON_PIN    = 2;
const int BUZZER_PIN    = 7;
const int GREEN_LED_PIN = 8;
const int RED_LED_PIN   = 9;
const int LDR_PIN       = A0;

// Calibrated Threshold Configurations
const float TEMP_HEAT_THRESHOLD_C  = 40.0; // Overheat alarm triggers above 35°C
const int LIGHT_INTRUDER_THRESHOLD = 300;  // Intruder alarm triggers when light drops below 300 ADC

DHT dht(DHTPIN, DHTTYPE);

enum SystemState {
  DISARMED,
  ARMED,
  ALARM_HEAT,
  ALARM_INTRUDER,
  ALARM_DUAL
};

SystemState currentState = DISARMED;

// Button Debounce Variables
bool lastButtonReading = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY_MS = 50;

// Non-blocking Timing Variables
unsigned long lastSensorSampleTime = 0;
const unsigned long SENSOR_INTERVAL_MS = 2000; // DHT11 2-second sampling interval

unsigned long lastSirenToggleTime = 0;
bool sirenPitchToggle = false;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  setSystemState(DISARMED);
  Serial.println("==============================================");
  Serial.println("   HEAT & INTRUDER ALARM SYSTEM INITIALIZED   ");
  Serial.println("==============================================");
}

void loop() {
  unsigned long currentMillis = millis();

  // -------------------------------------------------------------
  // 1. BUTTON INPUT HANDLING (Arm / Disarm Toggle)
  // -------------------------------------------------------------
  bool currentReading = digitalRead(BUTTON_PIN);

  if (currentReading != lastButtonReading) {
    lastDebounceTime = currentMillis;
  }

  if ((currentMillis - lastDebounceTime) > DEBOUNCE_DELAY_MS) {
    static bool debouncedButtonState = HIGH;
    if (currentReading == LOW && debouncedButtonState == HIGH) {
      handleButtonPress();
    }
    debouncedButtonState = currentReading;
  }
  lastButtonReading = currentReading;

  // -------------------------------------------------------------
  // 2. SENSOR SAMPLING (Evaluated every 2 seconds)
  // -------------------------------------------------------------
  if (currentMillis - lastSensorSampleTime >= SENSOR_INTERVAL_MS) {
    lastSensorSampleTime = currentMillis;

    int ldrValue = analogRead(LDR_PIN);
    float tempC = dht.readTemperature();

    if (isnan(tempC)) {
      Serial.println("Error: Failed to read from DHT11 sensor! Check Pin 3 wiring.");
    } else {
      // Print real-time diagnostic telemetry to Serial Monitor
      Serial.print("Light Level: ");
      Serial.print(ldrValue);
      Serial.print(" | Temp: ");
      Serial.print(tempC, 1);
      Serial.print(" °C | System State: ");
      printStateName(currentState);

      // Hazard Evaluation (Active when ARMED or already in ALARM)
      if (currentState != DISARMED) {
        bool isHeatTriggered = (tempC > TEMP_HEAT_THRESHOLD_C);
        bool isIntruderTriggered = (ldrValue < LIGHT_INTRUDER_THRESHOLD);

        if (isHeatTriggered && isIntruderTriggered) {
          setSystemState(ALARM_DUAL);
        } else if (isHeatTriggered) {
          setSystemState(ALARM_HEAT);
        } else if (isIntruderTriggered) {
          setSystemState(ALARM_INTRUDER);
        }
      }
    }
  }

  // -------------------------------------------------------------
  // 3. SIREN AUDITORY ALERTS (Non-blocking pitch generator)
  // -------------------------------------------------------------
  if (currentState == ALARM_HEAT || currentState == ALARM_INTRUDER || currentState == ALARM_DUAL) {
    if (currentMillis - lastSirenToggleTime >= 120) {
      lastSirenToggleTime = currentMillis;
      sirenPitchToggle = !sirenPitchToggle;

      if (currentState == ALARM_HEAT) {
        tone(BUZZER_PIN, sirenPitchToggle ? 1000 : 1500); // High-frequency thermal siren
      } else if (currentState == ALARM_INTRUDER) {
        tone(BUZZER_PIN, sirenPitchToggle ? 500 : 800);   // Low-frequency intruder siren
      } else {
        tone(BUZZER_PIN, sirenPitchToggle ? 600 : 1800);  // High-urgency dual hazard siren
      }
    }
  }
}

// -------------------------------------------------------------
// STATE MACHINE CONTROLLER
// -------------------------------------------------------------
void setSystemState(SystemState newState) {
  if (currentState == newState) return;
  currentState = newState;

  switch (currentState) {
    case DISARMED:
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
      noTone(BUZZER_PIN);
      Serial.println("\n[SYSTEM DISARMED] Standby Mode.");
      break;

    case ARMED:
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
      noTone(BUZZER_PIN);
      Serial.println("\n[SYSTEM ARMED] Active Surveillance Mode...");
      break;

    case ALARM_HEAT:
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);
      Serial.println("\n>>> HAZARD ALERT: OVERHEAT DETECTED! <<<");
      break;

    case ALARM_INTRUDER:
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);
      Serial.println("\n>>> HAZARD ALERT: INTRUDER SHADOW DETECTED! <<<");
      break;

    case ALARM_DUAL:
      digitalWrite(GREEN_LED_PIN, LOW);
      digitalWrite(RED_LED_PIN, HIGH);
      Serial.println("\n>>> CRITICAL ALERT: DUAL OVERHEAT & INTRUDER! <<<");
      break;
  }
}

void handleButtonPress() {
  if (currentState == DISARMED) {
    setSystemState(ARMED);
  } else {
    setSystemState(DISARMED); // Disarm immediately from any active state
  }
}

void printStateName(SystemState state) {
  switch (state) {
    case DISARMED:       Serial.println("DISARMED"); break;
    case ARMED:          Serial.println("ARMED"); break;
    case ALARM_HEAT:     Serial.println("ALARM (HEAT)"); break;
    case ALARM_INTRUDER: Serial.println("ALARM (INTRUDER)"); break;
    case ALARM_DUAL:     Serial.println("ALARM (CRITICAL DUAL)"); break;
  }
}