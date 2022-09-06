#include <Servo.h>

Servo steeringServo;

int startSteeringPos = 90;

void setup() {
  steeringServo.attach(2);
}

void loop() {
    steeringServo.write(startSteeringPos);//Startup Values
}
