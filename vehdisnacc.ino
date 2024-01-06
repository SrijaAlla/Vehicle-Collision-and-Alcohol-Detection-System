
// LCD displays distance of obstacle
// distance sensor is connected to arduino
// accelarometer senses angle along one direction
// **************** WORKKING WITH ONLY LCD , DISTANCE SENSOR , ACCELAROMETER  *********** 

#include <LiquidCrystal.h>
#include <Wire.h>


LiquidCrystal lcd(12,11,5,4,3,2) ;
int  trigPin = 10 ;   //trig
int  echoPin = 9 ;    //echo
//int  red     = 7 ;   
//int  green   = 8 ;
long duration , cm, inches ;
int piezopin = 8;

//Declaring some global variables accelerometer
int gyro_x, gyro_y, gyro_z;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
boolean set_gyro_angles;

long acc_x, acc_y, acc_z, acc_total_vector;
float angle_roll_acc, angle_pitch_acc;

float angle_pitch, angle_roll;
int angle_pitch_buffer, angle_roll_buffer;
float angle_pitch_output, angle_roll_output;

long loop_timer;
int temp;
int led = 13;                // the pin that the LED is atteched to
//int sensor = 2;              // the pin that the sensor is atteched to
//int state = LOW;             // by default, no motion detected
//int val = 0;                 // variable to store the sensor status (value)



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

  Wire.begin();                                                        //Start I2C as master
  setup_mpu_6050_registers();                                          //Setup the registers of the MPU-6050 
  for (int cal_int = 0; cal_int < 1000 ; cal_int ++){                  //Read the raw acc and gyro data from the MPU-6050 for 1000 times
    read_mpu_6050_data();                                             
    gyro_x_cal += gyro_x;                                              //Add the gyro x offset to the gyro_x_cal variable
    gyro_y_cal += gyro_y;                                              //Add the gyro y offset to the gyro_y_cal variable
    gyro_z_cal += gyro_z;                                              //Add the gyro z offset to the gyro_z_cal variable
    delay(3);                                                          //Delay 3us to have 250Hz for-loop
  }

  // divide by 1000 to get avarage offset
  gyro_x_cal /= 1000;                                                 
  gyro_y_cal /= 1000;                                                 
  gyro_z_cal /= 1000;                                                 
  Serial.begin(115200);
  loop_timer = micros(); 

  // pinMode(led, OUTPUT);      // initalize LED as an output
  //pinMode(sensor, INPUT);    // initialize sensor as an input
 // Serial.begin(9600);        // initialize serial

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

  read_mpu_6050_data();   
 //Subtract the offset values from the raw gyro values
  gyro_x -= gyro_x_cal;                                                
  gyro_y -= gyro_y_cal;                                                
  gyro_z -= gyro_z_cal;                                                
         
  //Gyro angle calculations . Note 0.0000611 = 1 / (250Hz x 65.5)
  angle_pitch += gyro_x * 0.0000611;                                   //Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_roll += gyro_y * 0.0000611;                                    //Calculate the traveled roll angle and add this to the angle_roll variable
  //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
  angle_pitch += angle_roll * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the roll angle to the pitch angel
  angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               //If the IMU has yawed transfer the pitch angle to the roll angel
  
  //Accelerometer angle calculations
  acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));  //Calculate the total accelerometer vector
  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
  angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;       //Calculate the pitch angle
  angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;       //Calculate the roll angle
  
  angle_pitch_acc -= 0.0;                                              //Accelerometer calibration value for pitch
  angle_roll_acc -= 0.0;                                               //Accelerometer calibration value for roll

  if(set_gyro_angles){                                                 //If the IMU is already started
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
  }
  else{                                                                //At first start
    angle_pitch = angle_pitch_acc;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle 
    angle_roll = angle_roll_acc;                                       //Set the gyro roll angle equal to the accelerometer roll angle 
    set_gyro_angles = true;                                            //Set the IMU started flag
  }
 // if(angle_roll >= 3.30 || angle_roll <= -3.30){
   // digitalWrite(10,HIGH);
  //}

  //if(angle_pitch >= 3.30 || angle_pitch <= -3.30){
    //digitalWrite(10,HIGH);
 // }
  //To dampen the pitch and roll angles a complementary filter is used
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;   //Take 90% of the output pitch value and add 10% of the raw pitch value
  angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;      //Take 90% of the output roll value and add 10% of the raw roll value
  Serial.print(" | Angle  = "); Serial.println(angle_pitch_output);

 while(micros() - loop_timer < 4000);                                 //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop
 loop_timer = micros();//Reset the loop timer

 //if(angle_roll >= 16.30 || angle_roll <= -4.00){
   // digitalWrite(13,HIGH);
  //}

  if(angle_pitch >= 2.00 || angle_pitch <= -2.00){
    digitalWrite(13,HIGH);
  }
// pir sensor
  /* val = digitalRead(sensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(led, HIGH);   // turn LED ON
    delay(100);                // delay 100 milliseconds 
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {
      digitalWrite(led, LOW); // turn LED OFF
      delay(200);             // delay 200 milliseconds 
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }*/

}

void setup_mpu_6050_registers(){
  //Activate the MPU-6050
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x6B);                                                    //Send the requested starting register
  Wire.write(0x00);                                                    //Set the requested starting register
  Wire.endTransmission();                                             
  //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1C);                                                    //Send the requested starting register
  Wire.write(0x10);                                                    //Set the requested starting register
  Wire.endTransmission();                                             
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1B);                                                    //Send the requested starting register
  Wire.write(0x08);                                                    //Set the requested starting register
  Wire.endTransmission();                                             
}


void read_mpu_6050_data(){                                             //Subroutine for reading the raw gyro and accelerometer data
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x3B);                                                    //Send the requested starting register
  Wire.endTransmission();                                              //End the transmission
  Wire.requestFrom(0x68,14);                                           //Request 14 bytes from the MPU-6050
  while(Wire.available() < 14);                                        //Wait until all the bytes are received
  acc_x = Wire.read()<<8|Wire.read();                                  
  acc_y = Wire.read()<<8|Wire.read();                                  
  acc_z = Wire.read()<<8|Wire.read();                                  
  temp = Wire.read()<<8|Wire.read();                                   
  gyro_x = Wire.read()<<8|Wire.read();                                 
  gyro_y = Wire.read()<<8|Wire.read();                                 
  gyro_z = Wire.read()<<8|Wire.read();                                 
}
