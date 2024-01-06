#include <LiquidCrystal.h>

#define beeper  8
#define sensor A0


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int gasLevel = 0;
String quality ="";

void setup() {
  Serial.begin(9600);
  pinMode(beeper,OUTPUT);
  pinMode(sensor,INPUT);

  lcd.begin(16, 2);
  lcd.setCursor (0,0);
  lcd.print("            "); 
  lcd.setCursor (0,1);
  lcd.print("            ");
  lcd.setCursor (0,0);
  lcd.print(" Air Sensor ");
  lcd.setCursor (0,1);
  lcd.print(" Warming Up ");
  delay(2000);

  lcd.setCursor (0,0);
  lcd.print("            ");
  lcd.setCursor (0,1);
  lcd.print("            ");

}

void loop() {
  gasLevel = analogRead(sensor);

  if(gasLevel<175) {
    quality = "GOOD!          ";
  }
  else if(gasLevel >175 && gasLevel<225){
    quality = "AVERAGE?       ";
  }
  else if(gasLevel >225 && gasLevel<300){
    quality = "ok?       ";
  }
  else if(gasLevel >300){
    quality = "PRETTY BAD!       ";
  }
  lcd.setCursor (0,0);
  lcd.print("AIR QUALITY IS:");
  lcd.setCursor (0,1);
  lcd.print(quality);
}
  
   
 
