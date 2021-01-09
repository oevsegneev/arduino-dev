const byte enPin = 3;
const byte dirPin[2] = {2,4};

void setup() {
  pinMode(enPin, OUTPUT);
  pinMode(dirPin[0], OUTPUT);
  pinMode(dirPin[1], OUTPUT);

  digitalWrite(dirPin[0], HIGH);
  digitalWrite(dirPin[1], LOW);
  analogWrite(enPin,255);
  delay(2000);
  analogWrite(enPin,0);
}

void loop() {
  digitalWrite(dirPin[0], HIGH);
  digitalWrite(dirPin[1], LOW);
  for(byte i=0; i<256; i++){
    analogWrite(enPin,i);
    delay(10);
  }
  for(byte i=0; i<256; i++){
    analogWrite(enPin,255-i);
    delay(10);
  }
  delay(1000);
  digitalWrite(dirPin[0], LOW);
  digitalWrite(dirPin[1], HIGH);
  for(byte i=0; i<256; i++){
    analogWrite(enPin,i);
    delay(10);
  }
  for(byte i=0; i<256; i++){
    analogWrite(enPin,255-i);
    delay(10);
  }
  delay(1000);
}
