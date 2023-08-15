// Include iBus Library
#include <IBusBM.h>
#include <Servo.h> 

// Create iBus Object
IBusBM ibus;

// Create Servo Object
Servo Servo1; 
Servo Servo2; 
Servo Servo3; 
Servo Servo4; 
Servo SeedDropper; // Servo to rotate and control the speed of seed dropping wheel

// defining pins for the weedicide spraying frame (for changing distance between nozzles and for opening/closing frame )
const int openframe = 26;                          
const int closeframe = 24;                        
const int enableframe = 28;

// Initial Channel Values
int rcCH1f = 0; // Left - Right rotation of front servo
int rcCH1b = 0; // Left - Right rotation of rear servo
int rcCH2 = 0; // Forward - Reverse
int rcCH3 = 0; // Acceleration
int rcCH4 = 0; // To Open/close the weedicide spraying frame
//int servoPin = 9; 
// Motor A Control Connections

#define in1A 3
#define pwmA 4
#define in1B 5
#define pwmB 6
//#define in2A 4
 

// Motor Speed Values - Start at zero
int MotorSpeedA = 0;
int MotorSpeedB = 0;
int servoangle1 = 0;
int servoangle2 = 0;
// Motor Direction Values - 0 = backward, 1 = forward
int MotorDirA = 1;
int MotorDirB = 1;
 
// Control Motor A
void mControlA(int mspeed, int mdir) {
 
  // Determine direction
  if (mdir == 0) {
    // Motor backward
    digitalWrite(in1A, LOW);
  } else {
    // Motor forward
    digitalWrite(in1A, HIGH);
  }
 
  // Control motor
  analogWrite(pwmA, mspeed);
 
}
void mControlB(int mspeed, int mdir) {
 
  // Determine direction
  if (mdir == 0) {
    // Motor backward
    digitalWrite(in1B, LOW);
  } else {
    // Motor forward
    digitalWrite(in1B, HIGH);
  }
 
  // Control motor
  analogWrite(pwmB, mspeed);
 
} 

 
// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 900) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}
  
void setup()
 
{
 
  // Start serial monitor for debugging
  Serial.begin(115200);
 
  // Attach iBus object to serial port
  ibus.begin(Serial1);
  
  // Set all the motor control & servo pins to outputs
  SeedDropper.attach(8);
  Servo1.attach(9); 
  Servo2.attach(10);
  Servo3.attach(11);
  Servo4.attach(12);
  
  pinMode(pwmA, OUTPUT);
  pinMode(in1A, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(in1B, OUTPUT);
  pinMode(openframe, OUTPUT);
  pinMode(closeframe, OUTPUT);
  pinMode(enableframe, OUTPUT);
}
 
void loop() {
 
  // Get RC channel values
  rcCH1f = readChannel(0, 0, 180, 90);
  rcCH1b = readChannel(0, 180, 0, 90);
  rcCH2 = readChannel(1, -100, 100, 0);
  rcCH3 = readChannel(2, 0, 155, 0);
  rcCH4 = readChannel(3, -100, 100, 0);

// Controlling the spraying frame with RC
  if(rcCH4<-50 && rcCH1f>100 && rcCH1f<115){
    digitalWrite(enableframe, HIGH);
    digitalWrite(closeframe, HIGH); 
    digitalWrite(openframe, LOW); 
    delay(100);
    }
   if(rcCH4>50 && rcCH1f>100 && rcCH1f<115){
    digitalWrite(enableframe, HIGH);
    digitalWrite(closeframe, LOW); 
    digitalWrite(openframe, HIGH);
    delay(100); 
    }
   digitalWrite(enableframe, LOW);
  // Print values to serial monitor for debugging
//  Serial.print("Ch1f = ");
//  Serial.print(rcCH1f);
//
//  Serial.print("Ch1b = ");
//  Serial.print(rcCH1b);
//  
//  Serial.print(" Ch2 = ");
//  Serial.print(rcCH2);
// 
//  Serial.print(" Ch3 = ");
//  Serial.print(rcCH3);

 
  // Set speeds with channel 3 value
  MotorSpeedA = rcCH3;
  MotorSpeedB = rcCH3;
 
    // Set forward/backward direction with channel 2 value
    if (rcCH2 >= 0) {
      //Forward
      MotorDirA = 1;
      MotorDirB = 1;
//      Serial.println("Forward");
    } else {
      //Backward
      MotorDirA = 0;
      MotorDirB = 0;
//      Serial.println("Backward");
    }
 
    // Add channel 2 speed
    MotorSpeedA = MotorSpeedA + abs(rcCH2);
    MotorSpeedB = MotorSpeedB + abs(rcCH2);
    
  // keep servos straight when the channel 1 stick is in the middle, if not then rotate to an angle
  if(rcCH1f>100 && rcCH1f<115){
    servoangle1 = 90;}
    else{
  servoangle1 = rcCH1f;
    }
  if(rcCH1b>65 && rcCH1b<75){
    servoangle2 = 90;}
    else{
  servoangle2 = rcCH1b;
    } 
   // Ensure that angles are between 45 and 135
  servoangle1 = constrain(servoangle1, 45, 135);
  servoangle2 = constrain(servoangle2, 45, 135);
  // Ensure that speeds are between 0 and 255
  MotorSpeedA = constrain(MotorSpeedA, 0, 255);
  MotorSpeedB = constrain(MotorSpeedB, 0, 255);
 
  //Drive Motors and write servos
  SeedDropper.write(30);
  Servo1.write(servoangle1);
  Servo2.write(servoangle1);
  Servo3.write(servoangle2);
  Servo4.write(servoangle2);
  
  Serial.print("Servoangle1 = ");
  Serial.print(servoangle1);
  Serial.print(" Servoangle2 = ");
  Serial.print(servoangle2);
  mControlA(MotorSpeedA, MotorDirA);
  mControlB(MotorSpeedB, MotorDirB);

  // Print speed values to serial monitor for debugging
//  Serial.print("Motor A Speed = ");
//  Serial.print(MotorSpeedA);
//  Serial.print("Motor B Speed = ");
//  Serial.print(MotorSpeedB);

  // Slight delay
  delay(100);
 
}
