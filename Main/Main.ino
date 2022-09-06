#include <Servo.h>

/*

Laser Emitter 
  Pin 3

Laser Reciever 
  Pin 4

Servo
  Pin 2

Reciever
  Pin
  
LEDs
  #1:5
  #2:6
  #3:7

*/

//Define Components & Pins
int laserEmitterPin = 3;
int laserRecier = 4;
Servo steeringServo;

//LEDs
int IndicatorLED1 = 5;
int IndicatorLED2 = 6;
int IndicatorLED3 = 7;

//Startup Values
int startSteeringPos = 90; //Servo can be set from 0-180 so 90 is the middle
int lives = 3; // Number of Lives at the start

void setup() {
  //Laser Setup
  pinMode(laserEmitterPin, OUTPUT); //Emiter
  pinMode(laserRecier, INPUT); //Reciever

  //Steering Servo Setup
  steeringServo.attach(2);

  //LED Indicators
  pinMode(IndicatorLED1, OUTPUT);
  pinMode(IndicatorLED2, OUTPUT);
  pinMode(IndicatorLED3, OUTPUT);

  //Init Functions
  steeringServo.write(startSteeringPos);  //Set steering to forwards
  digitalWrite(IndicatorLED1, HIGH);      //Enable LED Indicator 1
  digitalWrite(IndicatorLED2, HIGH);      //Enable LED Indicator 2
  digitalWrite(IndicatorLED3, HIGH);      //Enable LED Indicator 3
}

void loop() {
  if (lives < 1) {
    //Robot is Dead
    digitalWrite(IndicatorLED1, LOW);     //Disables LED Indicator 1
    digitalWrite(IndicatorLED2, LOW);     //Disables LED Indicator 2
    digitalWrite(IndicatorLED3, LOW);     //Disables LED Indicator 3
    digitalWrite(laserEmitterPin, LOW);   //Disables Laser Emitter
  }
  else {
    if (lives == 2) {
      //2 Lives Remaining
      digitalWrite(IndicatorLED3, LOW);   //Enable LED Indicator 3
    }
    else if (lives == 1) {
      //1 Life Remaining
      digitalWrite(IndicatorLED2, LOW);   //Disables LED Indicator 2
      digitalWrite(IndicatorLED3, LOW);   //Disables LED Indicator 3
    }

    //Hit Detection
    if (digitalRead(laserRecier) == true) {
      //Hit Detected
      lives = lives - 1;//Remove 1 Life
    }
    
    digitalWrite(laserEmitterPin, HIGH);//Turn on the Laser
  }
}
