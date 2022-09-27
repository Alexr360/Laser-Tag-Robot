#include <Servo.h>

//Define Components & Pins
Servo steeringServo;      //Servo Object
int steeringServoPin = 2; //Pin of servo motor
int laserEmitter = 3;     //Pin of Laser Emitter
int laserRecier = 4;      //Pin of Laser Reciever
int IndicatorLED1 = 8;    //Pin of LED1
int IndicatorLED2 = 9;    //Pin of LED2
int IndicatorLED3 = 10;   //Pin of LED3
int IndicatorLED4 = 11;   //Pin of LED4

//Startup Values
int startSteeringPos = 90; //Servo can be set from 0-180 so 90 is the middle
int lives = 3;             // Number of Lives at the start
int hitReg = false;          // Cooldown after taking a hit
bool alive = false;        //Robot is alive

void setup() {
  //Laser Setup
  pinMode(laserEmitter, OUTPUT);    //Emiter
  pinMode(laserRecier, INPUT);      //Reciever

  //Steering Servo Setup
  steeringServo.attach(steeringServoPin);

  //LED Indicators
  pinMode(IndicatorLED1, OUTPUT);
  pinMode(IndicatorLED2, OUTPUT);
  pinMode(IndicatorLED3, OUTPUT);
  pinMode(IndicatorLED4, OUTPUT);

  //Init Functions
  steeringServo.write(startSteeringPos);  //Set steering to forwards
  digitalWrite(IndicatorLED1, HIGH);      //Enable LED Indicator 1
  digitalWrite(IndicatorLED2, HIGH);      //Enable LED Indicator 2
  digitalWrite(IndicatorLED3, HIGH);      //Enable LED Indicator 3
  digitalWrite(IndicatorLED4, LOW);       //Disables LED Indicator 4
  digitalWrite(laserEmitter, HIGH);       //Enable Laser Emitter
}

void loop() {
  //Hit Detection
  if (digitalRead(laserRecier) == true && hitReg == false) {
    //Hit Detected
    lives = lives - 1;//Remove 1 Life
    alive = livesIndicator(lives);
    hitReg = true;
    digitalWrite(IndicatorLED4, HIGH);      //Disables LED Indicator 4
  }
  else if (digitalRead(laserRecier) == false && hitReg == true) {
    hitReg = false;
    digitalWrite(IndicatorLED4, LOW);      //Disables LED Indicator 4
  }
}

bool livesIndicator(int lives) {
  if (lives == 3) {
    //3 lives 
    digitalWrite(IndicatorLED1, HIGH);     //Enables LED Indicator 1
    digitalWrite(IndicatorLED2, HIGH);     //Enables LED Indicator 2
    digitalWrite(IndicatorLED3, HIGH);     //Enables LED Indicator 3
    digitalWrite(IndicatorLED4, LOW);      //Disables LED Indicator 4
    digitalWrite(laserEmitter, HIGH);      //Enables Laser Emitter
    return true;
  }
  else if (lives == 2) {
    //2 Lives
    digitalWrite(IndicatorLED1, HIGH);     //Enables LED Indicator 1
    digitalWrite(IndicatorLED2, HIGH);     //Enables LED Indicator 2
    digitalWrite(IndicatorLED3, LOW);      //Disables LED Indicator 3
    digitalWrite(IndicatorLED4, LOW);      //Disables LED Indicator 4
    digitalWrite(laserEmitter, HIGH);      //Enables Laser Emitter
    return true;
  }
  else if (lives == 1) {
    //1 Life
    digitalWrite(IndicatorLED1, HIGH);     //Enables LED Indicator 1
    digitalWrite(IndicatorLED2, LOW);      //Disables LED Indicator 2
    digitalWrite(IndicatorLED3, LOW);      //Disables LED Indicator 3
    digitalWrite(IndicatorLED4, LOW);      //Disables LED Indicator 4
    digitalWrite(laserEmitter, HIGH);      //Enables Laser Emitter
    return true;
  }
  else if (lives < 1) {
    //Robot is Dead
    digitalWrite(IndicatorLED1, LOW);     //Disables LED Indicator 1
    digitalWrite(IndicatorLED2, LOW);     //Disables LED Indicator 2
    digitalWrite(IndicatorLED3, LOW);     //Disables LED Indicator 3
    digitalWrite(IndicatorLED4, HIGH);     //Enables LED Indicator 4
    digitalWrite(laserEmitter, LOW);      //Disables Laser Emitter
    return false;
  }
}
