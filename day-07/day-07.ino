/*
 * Tinkercad Verified Code - Havilah Day 22
 */

const int btnMotorPin = 2;  // Pushbutton 1
const int btnRelayPin = 3;  // Pushbutton 2
const int relayPin    = 4;  // Relay Drive Pin
const int motorPwmPin = 5;  // Motor PWM Pin
const int ledPin      = 13; // Status LED
const int potPin      = A0; // Potentiometer
const int tmpPin      = A1; // TMP36 Sensor

bool motorState = false;
bool relayState = false;

bool lastBtn1State = HIGH;
unsigned long lastDebounceTime1 = 0;

bool lastBtn2State = HIGH;
unsigned long lastDebounceTime2 = 0;

const unsigned long debounceDelay = 50;
unsigned long lastPrintTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(btnMotorPin, INPUT_PULLUP);
  pinMode(btnRelayPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  pinMode(motorPwmPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(relayPin, LOW);
  analogWrite(motorPwmPin, 0);
  digitalWrite(ledPin, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Button 1 (Motor Toggle)
  bool readBtn1 = digitalRead(btnMotorPin);
  if (readBtn1 != lastBtn1State) {
    lastDebounceTime1 = currentMillis;
  }
  if ((currentMillis - lastDebounceTime1) > debounceDelay) {
    static bool debouncedBtn1 = HIGH;
    if (readBtn1 == LOW && debouncedBtn1 == HIGH) {
      motorState = !motorState;
    }
    debouncedBtn1 = readBtn1;
  }
  lastBtn1State = readBtn1;

  // 2. Button 2 (Relay Toggle)
  bool readBtn2 = digitalRead(btnRelayPin);
  if (readBtn2 != lastBtn2State) {
    lastDebounceTime2 = currentMillis;
  }
  if ((currentMillis - lastDebounceTime2) > debounceDelay) {
    static bool debouncedBtn2 = HIGH;
    if (readBtn2 == LOW && debouncedBtn2 == HIGH) {
      relayState = !relayState;
      digitalWrite(relayPin, relayState ? HIGH : LOW);
    }
    debouncedBtn2 = readBtn2;
  }
  lastBtn2State = readBtn2;

  // 3. Sensor Readings
  int potValue = analogRead(potPin);
  int manualPwm = map(potValue, 0, 1023, 0, 255);

  int rawTmp = analogRead(tmpPin);
  float voltage = rawTmp * (5.0 / 1023.0);
  float tempC = (voltage - 0.5) * 100.0;

  int autoPwm = 0;
  if (tempC < 25.0) {
    autoPwm = 0;
  } else if (tempC >= 35.0) {
    autoPwm = 255;
  } else {
    autoPwm = map((int)(tempC * 10), 250, 350, 0, 255);
    autoPwm = constrain(autoPwm, 0, 255);
  }

  // 4. Drive Motor
  int finalPwm = motorState ? manualPwm : autoPwm;
  analogWrite(motorPwmPin, finalPwm);
  digitalWrite(ledPin, (finalPwm > 0) ? HIGH : LOW);

  // 5. Serial Logging
  if (currentMillis - lastPrintTime >= 500) {
    lastPrintTime = currentMillis;
    int speedPercent = map(finalPwm, 0, 255, 0, 100);

    Serial.print("Temp: ");
    Serial.print(tempC, 1);
    Serial.print(" C | Relay: ");
    Serial.print(relayState ? "ON" : "OFF");
    Serial.print(" | Motor Mode: ");
    Serial.print(motorState ? "MANUAL" : "AUTO");
    Serial.print(" | Motor Speed: ");
    Serial.print(speedPercent);
    Serial.println("%");
  }
}