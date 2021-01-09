const int ledPinR = 2;
const int ledPinY = 3;
const int ledPinG = 4;

void setup() {
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinY, OUTPUT);
  pinMode(ledPinG, OUTPUT);
}

void loop() {
  digitalWrite(ledPinG, LOW);
  digitalWrite(ledPinR, HIGH);
  delay(200);
  digitalWrite(ledPinR, LOW);
  digitalWrite(ledPinY, HIGH);
  delay(200);              
  digitalWrite(ledPinY, LOW);
  digitalWrite(ledPinG, HIGH);
  delay(200);              
}
