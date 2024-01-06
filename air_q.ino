
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
lcd.print("AIR QUALITY =");
lcd.print(value);
Serial.print(value);
if (value> 700) 

{
    //digitalWrite(ledPin,HIGH);
    tone(piezopin, 1000);
     lcd.setCursor(0, 2);
     lcd.print("HAZARDOUS!!!");
     //while(1);
    Serial.print ("HAZARDOUS");   
  } 
  else if (value > 500 && value < 700 ){
  //  digitalWrite(ledPin,LOW); 
      noTone(piezopin);
      lcd.setCursor(0, 2);
      lcd.print("TOXIC");
    Serial.print("TOXIC");
  }
   
  else if (value > 200 && value < 500 ){
  //  digitalWrite(ledPin,LOW); 
      noTone(piezopin);
      lcd.setCursor(0, 2);
      lcd.print("LITTLE HARMFUL");
    Serial.print("LITTLE HARMFUL");
  }
  else if (value > 150 && value < 200 ){
  //  digitalWrite(ledPin,LOW); 
      noTone(piezopin);
      lcd.setCursor(0, 2);
      lcd.print("UNHEALTHY");
    Serial.print("UNHEALTHY");
  }

  else if (value > 100 && value < 150 ){
  //  digitalWrite(ledPin,LOW); 
      noTone(piezopin);
      lcd.setCursor(0, 2);
      lcd.print("NOT GOOD");
    Serial.print("NOT GOOD");
  }

  else if (value > 50 && value < 100 ){
  //  digitalWrite(ledPin,LOW); 
      noTone(piezopin);
      lcd.setCursor(0, 2);
      lcd.print("MODERATE");
    Serial.print("MODERATE");
  }

  else if (value > 0 && value < 50 ){
  //  digitalWrite(ledPin,LOW); 
      noTone(piezopin);
      lcd.setCursor(0, 2);
      lcd.print("GOOD");
    Serial.print("GOOD");
  }
  
  delay(1000);
lcd.clear();
}
