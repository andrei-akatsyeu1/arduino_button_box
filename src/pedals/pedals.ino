#include <Joystick.h>

//Joystick Setup
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
  2, 0,                  // Button Count, Hat Switch Count
  false, false, true,     // X, Y, & Z Axis
  true, true, false,     // Rx, Ry, or No Rz
  false, false,          // Rudder but no throttle
  false, false, false);  // No accelerator, brake, or steering;
int leftPedal = 0;
int rightPedal = 0;
int rudderAxis_ = 0;
bool inversLeft = false;
bool inversRight = true;
bool inversRudder = false;
int pedalButtonLevel = 200; //level to consider pedal button is pressed                 
const int MAX_MAPPING = 1024;
const int LEFT_PEDAL_BUTTON = 0;
const int RIGHT_PEDAL_BUTTON = 1;

void setup(){
  Serial.begin(9600); //Start serial monitoring
  Joystick.begin(); //Starts joystick
}
  
void loop(){
  
  leftPedal = analogRead(A1);

  //Serial.print("LPraw:");
  //Serial.print(leftPedal);
  //Serial.print(",");
  
  leftPedal = invers(map(leftPedal,500,890,0,MAX_MAPPING), inversLeft);//tune mapping min/max according to your hardware
  Joystick.setRxAxis(leftPedal);
  Joystick.setButton(LEFT_PEDAL_BUTTON, leftPedal > pedalButtonLevel);
  
  rightPedal = analogRead(A0);

  //Serial.print("RPraw:");
  //Serial.print(rightPedal);
  //Serial.print(",");
  
  rightPedal = invers(map(rightPedal,400,790,0,MAX_MAPPING), inversRight);//tune mapping min/max according to your hardware
  Joystick.setRyAxis(rightPedal);
  Joystick.setButton(RIGHT_PEDAL_BUTTON, rightPedal > pedalButtonLevel);

  rudderAxis_ = analogRead(A2);

  //Serial.print("RDraw:");
  //Serial.println(rudderAxis_);
  
  rudderAxis_ = invers(map(rudderAxis_,270,740,0,MAX_MAPPING), inversRudder);//tune mapping min/max according to your hardware
  Joystick.setZAxis(rudderAxis_);
/*
  Serial.print("LP:");
  Serial.print(leftPedal);
  Serial.print(",");
  Serial.print("RP:");
  Serial.print(rightPedal);
  Serial.print(",");
  Serial.print("RD:");
  Serial.println(rudderAxis_);
*/  
  delay(100);
}

int invers(int value, bool invers){
  if(invers){
    return MAX_MAPPING - value;
  } else {
    return value;  
  }
}
