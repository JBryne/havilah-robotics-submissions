/*
  Project: Smart Door with Lock
  Course: Week 5 Day 25 Mini Project
  
  Description:
  Automated door lock system using an HC-SR04 ultrasonic sensor,
  servo motor latch, status LEDs, piezo buzzer, and a manual override button.
*/

#include <Servo.h>

// --- Named Constants for Pins ---
const int TRIG_PIN      = 2;
const int ECHO_PIN      = 3;
const int BUTTON_PIN    = 4;
const int RED_LED_PIN   = 5;
const int GREEN_LED_PIN = 6;
const int BUZZER_PIN    = 7;
const int SERVO_PIN     = 9;

// --- System Thresholds & Parameters ---
const int UNLOCK_DISTANCE_CM = 15;  // Door unlocks if person is closer than 15cm
const int SERVO_LOCKED_POS   = 0;   // Degrees for locked state
const int SERVO_UNLOCKED_POS = 90;  // Degrees for unlocked state

// --- Global Objects and Variables ---
Servo doorLatch;
bool isOverrideLocked = false;
bool lastButtonState  = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50; // 50ms button debounce

void setup() {
  // Initialize Serial Monitor for testing and debugging
  Serial.begin(9600);
  Serial.println("--- Smart Door Lock System Initializing ---");

  // Configure Pin Modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Active LOW button setup
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Attach Servo Motor
  doorLatch.attach(SERVO_PIN);
  
  // Set Initial Hardware State (Locked)
  doorLatch.write(SERVO_LOCKED_POS);
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);
  
  Serial.println("System Ready. Status: LOCKED");
}

void loop() {
  // 1. Handle Manual Lock Override Button (Debounced)
  checkManualOverride();

  // 2. Read Sensor Input
  long distanceCM = readDistanceCM();

  // Print diagnostic readout to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distanceCM);
  Serial.print(" cm | Override State: ");
  Serial.println(isOverrideLocked ? "MANUAL_LOCK_ENGAGED" : "AUTO");

  // 3. Decision Logic & Actuation
  if (distanceCM <= UNLOCK_DISTANCE_CM && distanceCM > 0 && !isOverrideLocked) {
    unlockDoor();
  } else {
    lockDoor();
  }

  delay(100); // Main loop stability delay
}

// --- Helper Functions ---

// Measures target distance using HC-SR04 ultrasonic pulse
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) return 999; // Return out-of-range value if no echo
  
  return duration * 0.0343 / 2; // Speed of sound conversion
}

// Unlocks the door latch and alerts user
void unlockDoor() {
  doorLatch.write(SERVO_UNLOCKED_POS);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  
  // Short access chirp (non-blocking tone sequence)
  tone(BUZZER_PIN, 1000, 100); 
}

// Locks the door latch and sets indicator
void lockDoor() {
  doorLatch.write(SERVO_LOCKED_POS);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  noTone(BUZZER_PIN);
}

// Handles toggle button pressing to force system lock
void checkManualOverride() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading == LOW && lastButtonState == HIGH) {
      isOverrideLocked = !isOverrideLocked; // Toggle state
      Serial.print(">>> Override Toggled. Manual Lock: ");
      Serial.println(isOverrideLocked ? "ON" : "OFF");
      
      // Audible feedback for button press
      tone(BUZZER_PIN, 2000, 150);
    }
  }
  
  lastButtonState = reading;
}