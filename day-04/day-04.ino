/*
 * Smart Systems: Automatic Night Light with Hysteresis
 * Architecture: Sense (A0 LDR) -> Think (Hysteresis Logic) -> Act (Pin 13 LED)
 */

const int ldrPin = A0;
const int ledPin = 13;

// Hysteresis Thresholds (Adjust during calibration)
const int ON_THRESHOLD = 120;   // LED turns ON when reading drops below this
const int OFF_THRESHOLD = 200;  // LED turns OFF when reading rises above this

bool ledState = false; // Tracks current state of the LED

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // -------------------------------------------------------------
  // 1. SENSE: Read the environment voltage level
  // -------------------------------------------------------------
  int ldrValue = analogRead(ldrPin);

  // -------------------------------------------------------------
  // 2. THINK: Apply hysteresis logic to decide state
  // -------------------------------------------------------------
  if (ldrValue < ON_THRESHOLD) {
    ledState = true;  // It is dark -> turn ON
  } else if (ldrValue > OFF_THRESHOLD) {
    ledState = false; // It is bright -> turn OFF
  }
  // If ldrValue is between 300 and 400, maintain the previous ledState.

  // -------------------------------------------------------------
  // 3. ACT: Output the decision to the actuator
  // -------------------------------------------------------------
  if (ledState) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // -------------------------------------------------------------
  // DEBUG: Print LDR Reading & LED State to Serial Monitor every 1s
  // -------------------------------------------------------------
  Serial.print("LDR Reading: ");
  Serial.print(ldrValue);
  Serial.print(" | LED State: ");
  if (ledState) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }

  delay(1000); // Wait 1 second before next iteration
}