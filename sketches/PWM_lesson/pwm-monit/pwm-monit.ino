const byte potPin = A0;
const byte in1Pin = 4;
const byte in2Pin = 5;
const byte enPin = 3;

void setup() {
  pinMode(potPin, INPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enPin, OUTPUT);

  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
}

void loop() {
  int v = analogRead(potPin);
  float b = 255*(v/1023.0);
  analogWrite(enPin, int(b));
}
