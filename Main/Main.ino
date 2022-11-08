#include <Servo.h>                //Import Servo Library

//Define Components & Pins
Servo steeringServo;              // Servo Object

#define steeringServoPin 53       // Pin of servo motor
#define laserEmitter 2            // Pin of Laser Emitter
#define laserRecier 3             // Pin of Laser Reciever

#define IndicatorLED1 4           // Pin of LED1
#define IndicatorLED2 5           // Pin of LED2
#define IndicatorLED3 6           // Pin of LED3
#define IndicatorLED4 7           // Pin of LED4

#define SteeringControl 11        // Steering Input Pin
#define ThrottleControl 12        // Throttle Input Pin
#define ControllerButton 13       // Button Input Pin

#define MotorPWMSpeed 8           // Pin of Motor PWM Output
#define MotorIn1 9                // Pin of Motor Output 1
#define MotorIn2 10               // Pin of Motor Output 2

//Hardware Values (Do not change these)
int hitReg = false;               // Toggle for hit detection
bool alive = false;               // Robot is alive
int oldSteer = 90;                // Steering value last loop



//Calibration Values (Feel free to change these)
int startSteeringPos = 90;        // Starting position for steering servo                     (Default: 90)
int steeringSensitivity = 12;     // How many degrees change needed before moving the servo   (Default: 6)
int minTurnAngle = 0;             // Minimum angle of steering                                (Default: 0)
int maxTurnAngle = 180;           // Maximum angle of steering                                (Default: 180)
int lives = 3;                    // Number of Lives at the start                             (Default: 3)
int ledBrightness = 25;           // Chages brightness of LEDS                                (Default: 25)
int debugMode = false             // Enables or Disables Debug Mode                           (Default: false)



//Setup (Laser, LEDs, Servos, Controlls, Debug)
void setup() {
  //Laser Setup
  pinMode(laserEmitter, OUTPUT);            // Laser Emiter
  pinMode(laserRecier, INPUT);              // Laser Reciever

  //Steering Servo Setup
  steeringServo.attach(steeringServoPin);

  //LED Indicators
  pinMode(IndicatorLED1, OUTPUT);           //Led 1
  pinMode(IndicatorLED2, OUTPUT);           //Led 2
  pinMode(IndicatorLED3, OUTPUT);           //Led 3
  pinMode(IndicatorLED4, OUTPUT);           //Led 4

  //Startup Conditions
  steeringServo.write(startSteeringPos);    // Set steering to forwards
  led(IndicatorLED1, true);                 // Enables LED Indicator 1
  led(IndicatorLED2, true);                 // Enables LED Indicator 2
  led(IndicatorLED3, true);                 // Enables LED Indicator 3
  led(IndicatorLED4, false);                // Disables LED Indicator 4

  //Controller Inputs
  pinMode(SteeringControl, INPUT);          //Steering Input from Controller
  pinMode(ThrottleControl, INPUT);          //Throttle Input from Controller
  pinMode(ControllerButton, INPUT);         //Laser Button from Controller

  //Debug Output
  if (debugMode) {
    Serial.begin(9600);
  }
}



//Main logic loop (Lives, Controlls, Laser, Steering, Motor, LEDs)
void loop() {
  alive = (lives > 0);    //Test if alive

  //Stops the robot from functioning if not alive
  if (alive) {
    //Laser Control
    int button = pulseIn(ControllerButton, HIGH);                             //Gather usable data from button
    int laserButton = map(button, 900, 1900, LOW, HIGH);                      //Map data to LOW or HIGH
    digitalWrite(laserEmitter, laserButton);                                  //Turn the laser on or off based on button position

    //Throttle Control
    int throttle = pulseIn(ThrottleControl, HIGH);                            //Gather usable data from throttle pin
    int throttleAdj = map(map(throttle, 1125, 2000, 0, 25), 0, 25, 0, 255);   //Map throttle data to 0-255 in 25 intervals
    int throttleFWD = map(throttleAdj, 125, 255, 0, 255);                     //Seperate FWD throttle from Total Throttle and map to 0-255
    int throttleBKW = map(throttleAdj, 0, 110, 255, 0);                       //Seperate BKW throttle from Total Throttle and map to 0-255
    MotorControl(throttleAdj, throttleFWD, throttleBKW);                      //Set Motor to speed based on Total throttle and give the function the speeds for forwards & backwards

    //Steering Control
    int steering = pulseIn(SteeringControl, HIGH);                            //Gather usable data from steering pin
    int steeringAdj = trim((map(steering, 1340, 1600, 0, 180) - 16),180,0);   //Map steering data to 0-180 and then trim it down so that any higher or lower values are ignored
    int steeringDiff = oldSteer-steeringAdj;                                  //Set the diffreence in steering to saved steering value - current steering value

    //If the difference in steering is greater than the steering sensitivity or if it is at the min/max angles move the steering servo and update the saved steering value
    if (steeringDiff > (steeringSensitivity/2) || steeringDiff < -(steeringSensitivity/2) || steeringAdj == minTurnAngle || steeringAdj == maxTurnAngle || steeringAdj == startSteeringPos) {
      oldSteer = steeringAdj;                                                 //Set saved steering value to current steering value
      steeringServo.write(steeringAdj);                                       //Move servo to reflect change in steering
    }
    
    //Hit Detection
    if (digitalRead(laserRecier) == true && hitReg == false) {
      //Reciver started detecting a hit
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



//Function to change LED's based on lives remaining after hit
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
    led(IndicatorLED1, false);            //Disables LED Indicator 1
    led(IndicatorLED2, false);            //Disables LED Indicator 2
    led(IndicatorLED3, false);            //Disables LED Indicator 3
    led(IndicatorLED4, true);             //Enables LED Indicator 4
    digitalWrite(laserEmitter, LOW);      //Disables Laser Emitter
    return;
  }
}



//Function to Turn LED on & off with custom brightness
void led(int pin, bool enabledDisabled) {
  if (enabledDisabled) {
    //Turn on LED with custom brightness
    analogWrite(pin, ledBrightness);
    return;
  }
  else {
    //Turn off LED
    digitalWrite(pin, LOW);
    return;
  }
}



void MotorControl(int speed, int throttleForwards, int throttleBackwards) {
  if (speed > 125) {
    //Forward
    digitalWrite(MotorIn1, HIGH);                   //Set Motor to forwards
    digitalWrite(MotorIn2, LOW);
    analogWrite(MotorPWMSpeed, throttleForwards);   //Set Motor to forwards calabrated throttle speed
    return;
  }
  else if (speed < 110) {
    //Backward
    digitalWrite(MotorIn1, LOW);                    //Set Motor to backwards
    digitalWrite(MotorIn2, HIGH);
    analogWrite(MotorPWMSpeed, throttleBackwards);  //Set Motor to backwards calabrated throttle speed
    return;
  }
  else {
    //Stopped
    digitalWrite(MotorIn1, LOW);    //Turn off Motor
    digitalWrite(MotorIn2, LOW);
    return;
  }
}



//Trim int to inside max & min values
int trim(int value, int maxValue, int minValue) {
  if (value > maxValue) {
    return maxValue;
  }
  else if (value < minValue) {
    return minValue;
  }
  return value;
}
