#include <Servo.h>

int potentiometer = 2;
int servoPin = 3;
int angle = 0;
Servo servo;

void setup() {
  Serial.begin(9600);
  pinMode(potentiometer, INPUT);
  servo.attach(servoPin);
}

void loop() {
  int angle = analogRead(potentiometer);
  angle = angle-355;
  servo.write(angle);
  
  Serial.println(angle);
  delay(1);
}
