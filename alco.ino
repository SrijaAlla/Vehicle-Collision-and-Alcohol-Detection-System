#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int ledPin = 10;
int sensorPin = A0;
int value;
int piezo = 8;

void setup()
{
Serial.begin(9600);
lcd.begin(16,2);
pinMode(ledPin,OUTPUT);
}
void loop() 
{
int Value = analogRead(sensorPin);
value = analogRead(A0);
lcd.print("Alcohol Lev.:");
lcd.print(value-50);
Serial.print(value);
if (value-50 > 300) 

{
  digitalWrite(ledPin,HIGH);
     tone(piezo, 1000);
     lcd.setCursor(0, 2);
     lcd.print("Alert....!!!");
    Serial.print ("Alert");   
  } 
  else {
    digitalWrite(ledPin,LOW);
      noTone(piezo); 
      lcd.setCursor(0, 2);
      lcd.print(".....Normal.....");
    Serial.print("Normal");
  }

delay(500);
lcd.clear();
}
