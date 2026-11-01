/*
 * LCD 16x2 Weather Station with Potentiometer Contrast Control
 * Pin Mapping:
 *   - Potentiometer Wiper -> LCD V0 (Pin 3)
 *   - LCD: RS=12, E=11, D4=5, D5=4, D6=3, D7=2
 *   - DHT11 Data -> Pin 7
 *   - Pushbutton -> Pin 8 (INPUT_PULLUP)
 *   - Red LED -> Pin 6
 */

#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11

const int redLedPin = 6;
const int buttonPin = 8;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DHT dht(DHTPIN, DHTTYPE);

bool displayFahrenheit = false;

bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long lastSensorReadTime = 0;
const unsigned long sensorInterval = 2000;

float currentTempC = 0.0;
float currentHumidity = 0.0;
bool sensorValid = false;

void setup() {
  Serial.begin(9600);

  pinMode(redLedPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(redLedPin, LOW);

  lcd.begin(16, 2);
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Hello, Robotics!");
  lcd.setCursor(0, 1);
  lcd.print("Init Weather...");
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Button Toggle Logic
  bool reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = currentMillis;
  }
  if ((currentMillis - lastDebounceTime) > debounceDelay) {
    static bool debouncedState = HIGH;
    if (reading == LOW && debouncedState == HIGH) {
      displayFahrenheit = !displayFahrenheit;
      updateDisplay();
    }
    debouncedState = reading;
  }
  lastButtonState = reading;

  // 2. DHT11 Reading (Every 2 seconds)
  if (currentMillis - lastSensorReadTime >= sensorInterval) {
    lastSensorReadTime = currentMillis;

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
      sensorValid = false;
      Serial.println("Error reading DHT11 sensor on Pin 7!");
      lcd.setCursor(0, 0);
      lcd.print("Sensor Error!   ");
      lcd.setCursor(0, 1);
      lcd.print("Check Wiring    ");
    } else {
      sensorValid = true;
      currentTempC = t;
      currentHumidity = h;

      Serial.print("Temp: ");
      Serial.print(currentTempC, 1);
      Serial.print(" C | Humidity: ");
      Serial.print(currentHumidity, 1);
      Serial.println(" %");

      digitalWrite(redLedPin, (currentTempC > 30.0) ? HIGH : LOW);
      updateDisplay();
    }
  }
}

void updateDisplay() {
  if (!sensorValid) return;

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");

  if (displayFahrenheit) {
    float tempF = (currentTempC * 9.0 / 5.0) + 32.0;
    lcd.print(tempF, 1);
    lcd.print(" F ");
  } else {
    lcd.print(currentTempC, 1);
    lcd.print(" C ");
  }

  if (currentTempC > 30.0) {
    lcd.print("Too Hot!");
  } else {
    lcd.print("Nice    ");
  }

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print((int)currentHumidity);
  lcd.print(" %   ");
}