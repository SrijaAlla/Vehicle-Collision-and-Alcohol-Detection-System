
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int ledPin = 10;
int sensorPin = A0;
int piezopin = 8;
int value;

void setup()
{
Serial.begin(9600);
lcd.begin(16,2);
//pinMode(ledPin,OUTPUT);

}
void loop() 
{
int Value = analogRead(sensorPin);
value = analogRead(A0);
lcd.print("Alcohol Lev.:");
lcd.print(value-50);
Serial.print(value);
if (value-50 > 250) 

{
    //digitalWrite(ledPin,HIGH);
    tone(piezopin, 1000);
     lcd.setCursor(0, 2);
     lcd.print("Alcohol Consumed!!!");
     //while(1);
    Serial.print ("Drunk");   
  } 
  else {
  //  digitalWrite(ledPin,LOW); 
      noTone(piezopin);
      lcd.setCursor(0, 2);
      lcd.print("Alcohol Not Consumed");
    Serial.print("Not Drunk");
  }

delay(1000);
lcd.clear();
}
