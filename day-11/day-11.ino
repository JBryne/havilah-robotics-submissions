// Import standard parallel LCD library
#include <LiquidCrystal.h>

// HC-SR04 Ultrasonic Sensor Pins
const int trigPin = 9;
const int echoPin = 10;

// Initialize standard 16x2 LCD in 4-bit mode: (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16, 2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Send 10us ultrasonic trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse duration (30ms timeout)
  long echoTime = pulseIn(echoPin, HIGH, 30000);

  if (echoTime == 0) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("NO ECHO");
  } else {
    float distanceCM = echoTime * 0.0343 / 2.0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DISTANCE");
    lcd.setCursor(9, 0);
    lcd.print(distanceCM, 1); // Display distance to 1 decimal place
    lcd.print("cm");
  }

  delay(500);
}