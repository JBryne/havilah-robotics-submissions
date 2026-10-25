/*
 * Day 20: Smart Systems - Option A: Smart Alarm (Upload-Safe Version)
 * Architecture: Sense (HC-SR04 Distance) -> Think (Threshold Logic) -> Act (Buzzer)
 */

const int trigPin = 2;
const int echoPin = 3;
const int buzzerPin = 8;

const int ALARM_THRESHOLD_CM = 30; // Alarm triggers at 30 cm or closer

void setup() {
  // 1. Startup Delay: Gives the CH340 bootloader 2 seconds to catch uploads on reset
  delay(2000); 

  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  digitalWrite(buzzerPin, LOW); // Start with alarm silent
}

void loop() {
  // 2. SENSE: Emit 10us ultrasonic trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure pulse return duration
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate distance in cm: d = (duration * 0.034) / 2
  int distance = (duration * 0.034) / 2;

  // 3. THINK & ACT: Evaluate threshold and log
  if (distance > 0 && distance <= ALARM_THRESHOLD_CM) {
    digitalWrite(buzzerPin, HIGH); // Sound alarm
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm | Status: ALARM TRIGGERED!");
  } else {
    digitalWrite(buzzerPin, LOW);  // Silence alarm
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm | Status: CLEAR");
  }

  delay(300); // 300ms pacing prevents COM port flooding
}