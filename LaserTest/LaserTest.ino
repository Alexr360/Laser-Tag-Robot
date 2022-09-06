int laserEmitterPin = 3;
void setup() {
  // put your setup code here, to run once:
  pinMode(laserEmitterPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(laserEmitterPin, HIGH);
  delay(1000);
  digitalWrite(laserEmitterPin, LOW);
  delay(1000);
}
