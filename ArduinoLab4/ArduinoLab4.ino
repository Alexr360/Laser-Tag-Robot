#define ControllerButton 13     //Button Input Pin
#define laserEmitter 2     //Pin of Laser Emitter

void setup() {
  pinMode(laserEmitter, OUTPUT);    //Emiter
  pinMode(ControllerButton, INPUT); //Controller
}

void loop() {
  int button = pulseIn(ControllerButton, HIGH);
  int buttonAdj = map(button, 900, 1900, 0, 1);
  
  if (buttonAdj) {
    digitalWrite(laserEmitter, HIGH);
  }
  else {
    digitalWrite(laserEmitter, LOW);
  }
}
