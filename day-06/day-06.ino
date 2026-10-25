/*
 * Full Temperature Alarm System using DHT11 Digital Sensor
 * Architecture: State Machine (DISARMED -> ARMED -> ALARM)
 * Pinout:
 *   - DHT11 Signal 'S': Pin 3
 *   - Pushbutton:       Pin 2 (Uses INPUT_PULLUP to GND)
 *   - Buzzer (+):       Pin 7
 *   - Green LED:        Pin 8 (via 220Ω resistor to GND)
 *   - Red LED:          Pin 9 (via 220Ω resistor to GND)
 */

#include <DHT.h>

#define DHTPIN 3        // DHT11 Signal pin 'S'
#define DHTTYPE DHT11

const int buttonPin   = 2;
const int buzzerPin   = 7;
const int greenLedPin = 8;
const int redLedPin   = 9;

const float ALARM_TEMP_THRESHOLD = 25.0; // Alarm triggers above 30°C

DHT dht(DHTPIN, DHTTYPE);

enum SystemState {
  DISARMED,
  ARMED,
  ALARM
};

SystemState currentState = DISARMED;

// Button Debounce & Edge Detection Variables
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Non-blocking Timing Variables
unsigned long lastSensorReadTime = 0;
unsigned long lastSirenToggleTime = 0;
bool sirenPitchToggle = false;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  setSystemState(DISARMED);
}

void loop() {
  unsigned long currentMillis = millis();

  // -------------------------------------------------------------
  // 1. BUTTON HANDLING (Debounced Press Detection)
  // -------------------------------------------------------------
  bool reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = currentMillis;
  }

  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    static bool debouncedState = HIGH;
    if (reading == LOW && debouncedState == HIGH) {
      handleButtonPress();
    }
    debouncedState = reading;
  }
  lastButtonState = reading;

  // -------------------------------------------------------------
  // 2. SENSOR SAMPLING (Reads DHT11 every 2 seconds)
  // -------------------------------------------------------------
  if (currentMillis - lastSensorReadTime >= 2000) {
    lastSensorReadTime = currentMillis;

    float tempC = dht.readTemperature();

    if (!isnan(tempC)) {
      Serial.print("Temperature: ");
      Serial.print(tempC, 1);
      Serial.println(" C");

      if (currentState == ARMED && tempC > ALARM_TEMP_THRESHOLD) {
        setSystemState(ALARM);
      }
    } else {
      Serial.println("Error: Failed to read from DHT11 sensor! Check wiring.");
    }
  }

  // -------------------------------------------------------------
  // 3. SIREN SOUND GENERATOR (Active while in ALARM state)
  // -------------------------------------------------------------
  if (currentState == ALARM) {
    if (currentMillis - lastSirenToggleTime >= 150) {
      lastSirenToggleTime = currentMillis;
      sirenPitchToggle = !sirenPitchToggle;

      if (sirenPitchToggle) {
        tone(buzzerPin, 1200);
      } else {
        tone(buzzerPin, 800);
      }
    }
  }
}

// -------------------------------------------------------------
// STATE MACHINE CONTROLLER
// -------------------------------------------------------------
void setSystemState(SystemState newState) {
  currentState = newState;

  switch (currentState) {
    case DISARMED:
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, LOW);
      noTone(buzzerPin);
      Serial.println(">>> STATE: DISARMED <<<");
      break;

    case ARMED:
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(redLedPin, LOW);
      noTone(buzzerPin);
      Serial.println(">>> STATE: ARMED <<<");
      break;

    case ALARM:
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
      Serial.println(">>> STATE: ALARM! <<<");
      break;
  }
}

// -------------------------------------------------------------
// BUTTON PRESS ACTION
// -------------------------------------------------------------
void handleButtonPress() {
  if (currentState == DISARMED) {
    setSystemState(ARMED);
  } else if (currentState == ARMED || currentState == ALARM) {
    setSystemState(DISARMED);
  }
}