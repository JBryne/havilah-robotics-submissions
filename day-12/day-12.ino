#include <DHT.h>
#include <LiquidCrystal.h>


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Declare Global variables
#define passBuzzer 8
#define redLed 7
#define dhtPin 9
#define DHTTYPE DHT11

const float tempLimit = 30.0;
const float tempReset = 25.0;

bool alarmOn = false;

DHT dht(dhtPin, DHTTYPE);

void setup(){
  pinMode(redLed, OUTPUT);
  pinMode(passBuzzer, OUTPUT);
  pinMode(dhtPin, INPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Digital Thermo");
  lcd.setCursor(0,1);
  lcd.print("Starting.......");
  delay(4000);

}
void loop(){
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if(isnan(t) || isnan(h)){
    lcd.setCursor(0,0);
    lcd.print("ERROR");
    lcd.setCursor(0,1);
    lcd.print("CHECK ERROR");
    //delay(3000);
    //kcd.clear();

    return;
  }
  if (t > tempLimit){
    alarmOn = true; //true
  }

    else if (t <= tempReset){
      alarmOn = false;
    }
  if (alarmOn){
    digitalWrite(redLed, HIGH);
    digitalWrite(passBuzzer, HIGH);
  }
  else {
    digitalWrite(redLed, LOW);
    digitalWrite(passBuzzer, LOW);
   }

    }
  

