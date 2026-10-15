#include <Servo.h>

const int ldrPin = A0;
const int buzzer = 8;
const int servoPin = 9;

int threshold = 500; // Temporary threshold
Servo myServo;

void setup() {
  Serial.begin(9600);       // Start Serial Monitor
  pinMode(buzzer, OUTPUT);
  myServo.attach(servoPin);
  myServo.write(0);
}

void loop() {
  int ldrValue = analogRead(ldrPin);

  // Print live value to computer screen
  Serial.print("LDR Reading: ");
  Serial.println(ldrValue);

  if (ldrValue < threshold) {
    myServo.write(90);
    digitalWrite(buzzer, HIGH);

    delay(500);

    digitalWrite(buzzer, LOW);
    myServo.write(0);

    delay(500);
  } else {
    delay(200);
  }
}