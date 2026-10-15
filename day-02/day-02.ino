int ldr = A0;
int led = 8;
int threshold = 500;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  int ldrValue = analogRead(ldr);

  Serial.print("LDR Reading: ");
  Serial.println(ldrValue);

  if (ldrValue < threshold) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }

  delay(500);
}