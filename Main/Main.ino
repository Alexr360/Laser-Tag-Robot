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
  
LED
  Pin 5
 */

//Define Components & Pins
int laserEmitterPin = 3;
int laserRecier = 4;
Servo steeringServo;

//LEDs
int IndicatorLED = 5;

//Startup Values
int startSteeringPos = 90;
int lives = 3;

void setup() {
  //Laser Setup
  pinMode(laserEmitterPin, OUTPUT); //Emiter
  pinMode(laserRecier, INPUT); //Reciever

  //Steering Servo Setup
  steeringServo.attach(2);
  steeringServo.write(startSteeringPos);

  //LED Output
  pinMode(IndicatorLED, OUTPUT);
}

void loop() {
  int laserState = digitalRead(laserRecier);

  if (laserState == true) {
    digitalWrite(IndicatorLED, HIGH);
    delay(500);
    digitalWrite(IndicatorLED, LOW);
    //Hit Detected
    lives = lives-1;
    if (lives < 1) {
      digitalWrite(IndicatorLED, HIGH);
      //Robot is Dead
      digitalWrite(laserEmitterPin, LOW);
    }
  }
  
  digitalWrite(laserEmitterPin, HIGH);
}
