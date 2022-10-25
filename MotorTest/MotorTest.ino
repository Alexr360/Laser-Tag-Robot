#define MotorPWMSpeed 8
#define MotorIn1 9
#define MotorIn2 10

void setup() {
  pinMode(MotorPWMSpeed, OUTPUT);
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);
}

void loop() {
  MotorControl(10);
}

void MotorControl(int speed) {
  if (speed > 127) {
    //Forward
    digitalWrite(MotorIn1, HIGH);
    digitalWrite(MotorIn2, LOW);
    analogWrite(MotorPWMSpeed, 255);
  }
  else if (speed == 127) {
    //Stopped
    digitalWrite(MotorIn1, LOW);
    digitalWrite(MotorIn2, LOW);
  }
  else {
    //Backward
    digitalWrite(MotorIn1, LOW);
    digitalWrite(MotorIn2, HIGH);
    analogWrite(MotorPWMSpeed, 255);
  }
}
