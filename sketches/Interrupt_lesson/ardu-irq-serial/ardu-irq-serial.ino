const byte interruptPin = 2;
long int rot = 0;

void detect() {
    rot++;  
}

void setup() {
    Serial.begin(9600);
  
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), detect, RISING);
}

void loop() {
  Serial.println(rot);
  delay(100);
}
