#include <LiquidCrystal.h>
#include <GSM.h>

#define PINNUMBER ""

// initialize the library instance
GSM gsmAccess; // include a 'true' parameter for debug enabled
GSM_SMS sms;

LiquidCrystal lcd(12,11,5,4,3,2) ;

char remoteNumber[20]= "+919908782869";  

char txtMsg[200]="ALERT -- Collision";

int  trigPin = 10 ;   //trig
int  echoPin = 9 ;    //echo
//int  red     = 7 ;   
//int  green   = 8 ;
long duration , cm, inches ;
int piezopin = 13;

void setup() {
  Serial.begin (9600) ;
 
  
  pinMode(trigPin, OUTPUT) ;
  pinMode(echoPin, INPUT) ;
  //initializing pins
  //get up the lcd's number of columns and rows 
  lcd.begin(16,2) ;
  //initialize digital pin 08 and 07 as an output
  pinMode(8, OUTPUT);
  //pinMode(7, OUTPUT);

  Serial.println("SMS Messages Sender");

  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  Serial.println("GSM initialized");
  sendSMS();
 
}

void loop() {
  //the sensor is triggered by a High pulse of 10 or more microseconds
  digitalWrite(trigPin, LOW) ;
  delayMicroseconds(5) ;
  digitalWrite(trigPin, HIGH) ;
  delayMicroseconds(10) ;
  digitalWrite(trigPin, LOW) ;
  // Read signal from sensor
  pinMode(echoPin, INPUT) ;
  duration = pulseIn(echoPin, HIGH) ;
  //convert the time into a distance
  cm = (duration/2) /29.1 ;
  delay(100);

  if ( cm >= 30 ) 
  {
    lcd.print(cm) ;
    lcd.print(" cm""-safe") ;
    //digitalWrite(8, HIGH) ; //turn the LED on (High voltage level)
    //delay(500);
    digitalWrite(8, LOW) ;
    noTone(piezopin) ;
    lcd.print("       ") ;
  
  }
  else if( cm <= 20 )
  {
    lcd.print(cm) ;
    lcd.print(" cm""DANGER") ;
    //digitalWrite(7, HIGH) ; //turn the LED on (High voltage level)
    tone(piezopin, 1000);
    
    delay(500);
    lcd.print("        ") ;  
  }
  lcd.setCursor(0,0) ;
  digitalWrite(8, LOW) ;
  //digitalWrite(7, LOW) ;

 

}

void sendSMS(){

  Serial.print("Message to mobile number: ");
  Serial.println(remoteNumber);

  // sms text
  Serial.println("SENDING");
  Serial.println();
  Serial.println("Message:");
  Serial.println(txtMsg);

  // send the message
  sms.beginSMS(remoteNumber);
  sms.print(txtMsg);
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");  
}
