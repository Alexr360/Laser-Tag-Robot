#include <Servo.h>

//Define Components & Pins
Servo steeringServo;       //Servo Object
#define steeringServoPin 2 //Pin of servo motor
#define laserEmitter 3     //Pin of Laser Emitter
#define laserRecier 4      //Pin of Laser Reciever
#define IndicatorLED1 8    //Pin of LED1
#define IndicatorLED2 9    //Pin of LED2
#define IndicatorLED3 10   //Pin of LED3
#define IndicatorLED4 11   //Pin of LED4

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
  steeringServo.write(startSteeringPos);    //Set steering to forwards
  led(IndicatorLED1, true);                 //Enables LED Indicator 1
  led(IndicatorLED2, true);                 //Enables LED Indicator 2
  led(IndicatorLED3, true);                 //Enables LED Indicator 3
  led(IndicatorLED4, false);                //Disables LED Indicator 4
  digitalWrite(laserEmitter, HIGH);         //Enable Laser Emitter
}

void loop() {
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

void livesIndicator(int lives) {
  if (lives == 3) {
    //3 lives 
    led(IndicatorLED1, true);             //Enables LED Indicator 1
    led(IndicatorLED2, true);             //Enables LED Indicator 2
    led(IndicatorLED3, true);             //Enables LED Indicator 3
    led(IndicatorLED4, false);            //Disables LED Indicator 4
    digitalWrite(laserEmitter, HIGH);     //Enables Laser Emitter
    return;
  }
  else if (lives == 2) {
    //2 Lives
    led(IndicatorLED1, true);             //Enables LED Indicator 1
    led(IndicatorLED2, true);             //Enables LED Indicator 2
    led(IndicatorLED3, false);            //Disables LED Indicator 3
    led(IndicatorLED4, false);            //Disables LED Indicator 4
    digitalWrite(laserEmitter, HIGH);     //Enables Laser Emitter
    return;
  }
  else if (lives == 1) {
    //1 Life
    led(IndicatorLED1, true);             //Enables LED Indicator 1
    led(IndicatorLED2, false);            //Disables LED Indicator 2
    led(IndicatorLED3, false);            //Disables LED Indicator 3
    led(IndicatorLED4, false);            //Disables LED Indicator 4
    digitalWrite(laserEmitter, HIGH);     //Enables Laser Emitter
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
