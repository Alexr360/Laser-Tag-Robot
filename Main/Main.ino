#include <Servo.h>

//Define Components & Pins
Servo steeringServo;       //Servo Object

#define steeringServoPin 53 //Pin of servo motor
#define laserEmitter 2     //Pin of Laser Emitter
#define laserRecier 3      //Pin of Laser Reciever

#define IndicatorLED1 4    //Pin of LED1
#define IndicatorLED2 5    //Pin of LED2
#define IndicatorLED3 6    //Pin of LED3
#define IndicatorLED4 7    //Pin of LED4

#define SteeringControll 11     //Steering Input Pin
#define ThrottleControll 12     //Throttle Input Pin
#define ControllerButton 13     //Button Input Pin

#define MotorPWMSpeed 8
#define MotorIn1 9
#define MotorIn2 10

//Hardware Values (Change Not reccomended)
int hitReg = false;       // Toggle for hit detection
bool alive = false;       // Robot is alive

//Customization Values (You can change)
int startSteeringPos = 90; //Servo can be set from 0-180 so 90 is the middle
int lives = 3;             // Number of Lives at the start
int ledBrightness = 25;   // Brightness of LEDS

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
//  steeringServo.write(startSteeringPos);    //Set steering to forwards
  led(IndicatorLED1, true);                 //Enables LED Indicator 1
  led(IndicatorLED2, true);                 //Enables LED Indicator 2
  led(IndicatorLED3, true);                 //Enables LED Indicator 3
  led(IndicatorLED4, false);                //Disables LED Indicator 4

  //Controller
  pinMode(SteeringControll, INPUT);
  pinMode(ThrottleControll, INPUT);
  pinMode(ControllerButton, INPUT);

  //Debug
  Serial.begin(9600);
}

void loop() {
  alive = (lives > 0);

  if (alive) {
    //Laser Control
    int button = pulseIn(ControllerButton, HIGH);
    int buttonAdj = map(button, 900, 1900, LOW, HIGH);
    digitalWrite(laserEmitter, buttonAdj);

    //Throttle Control
    int throttle = pulseIn(ThrottleControll, HIGH);
    int throttleAdj = map(map(throttle, 1125, 2000, 0, 25), 0, 25, 0, 255);
    int throttleFWD = map(throttleAdj, 125, 255, 0, 255);
    int throttleBKW = map(throttleAdj, 0, 110, 255, 0);
    MotorControl(throttleAdj, throttleFWD, throttleBKW);

    //Steering Control
    int steering = pulseIn(SteeringControll, HIGH);
    int steeringAdj = map(steering, 1330, 1660, 0, 180);
    steeringServo.write(steeringAdj);

//    Serial.println(throttleAdjB);
    
    //Hit Detection
    if (digitalRead(laserRecier) == true && hitReg == false) {
      //Reciver is being hit
      lives = lives - 1;                         //Remove 1 Life
      livesIndicator(lives);                     //Change LED's to Reflect Life Count
      hitReg = true;                             //Turn on the Being Hit Variable
      led(IndicatorLED4, true);                  //Disables LED Indicator 4
    }
    else if (digitalRead(laserRecier) == false && hitReg == true) {
      //Reciver has stopped being hit
      hitReg = false;                            //Turn off the being hit variable
      led(IndicatorLED4, false);                 //Disables LED Indicator 4
    } 
  }
}

void livesIndicator(int lives) {
  if (lives == 3) {
    //3 lives 
    led(IndicatorLED1, true);             //Enables LED Indicator 1
    led(IndicatorLED2, true);             //Enables LED Indicator 2
    led(IndicatorLED3, true);             //Enables LED Indicator 3
    led(IndicatorLED4, false);            //Disables LED Indicator 4
    return;
  }
  else if (lives == 2) {
    //2 Lives
    led(IndicatorLED1, true);             //Enables LED Indicator 1
    led(IndicatorLED2, true);             //Enables LED Indicator 2
    led(IndicatorLED3, false);            //Disables LED Indicator 3
    led(IndicatorLED4, false);            //Disables LED Indicator 4
    return;
  }
  else if (lives == 1) {
    //1 Life
    led(IndicatorLED1, true);             //Enables LED Indicator 1
    led(IndicatorLED2, false);            //Disables LED Indicator 2
    led(IndicatorLED3, false);            //Disables LED Indicator 3
    led(IndicatorLED4, false);            //Disables LED Indicator 4
    return;
  }
  else if (lives < 1) {
    //Robot is Dead
    led(IndicatorLED1, false);          //Disables LED Indicator 1
    led(IndicatorLED2, false);          //Disables LED Indicator 2
    led(IndicatorLED3, false);          //Disables LED Indicator 3
    led(IndicatorLED4, true);           //Enables LED Indicator 4
    digitalWrite(laserEmitter, LOW);    //Disables Laser Emitter
    return;
  }
}

void led(int pin, bool enabledDisabled) {
  //Custom Function to change led
  if (enabledDisabled == true) {
    analogWrite(pin, ledBrightness);
    return;
  }
  else {
    digitalWrite(pin, LOW);
    return;
  }
}

void MotorControl(int speed, int throttleAdjF, int throttleAdjB) {
  if (speed > 125) {
    //Forward
    digitalWrite(MotorIn1, HIGH);
    digitalWrite(MotorIn2, LOW);
    analogWrite(MotorPWMSpeed, throttleAdjF);
  }
  else if (speed < 110) {
    //Backward
    digitalWrite(MotorIn1, LOW);
    digitalWrite(MotorIn2, HIGH);
    analogWrite(MotorPWMSpeed, throttleAdjB);
  }
  else {
    //Stopped
    digitalWrite(MotorIn1, LOW);
    digitalWrite(MotorIn2, LOW);
  }
}
