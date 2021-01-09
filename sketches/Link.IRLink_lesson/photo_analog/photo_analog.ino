const byte photoPin = A0;

void setup() {
  Serial.begin(9600);
  pinMode(photoPin, INPUT);
}

void loop() {
  int v = analogRead(photoPin);
  Serial.println(v);
  delay(100);
}
