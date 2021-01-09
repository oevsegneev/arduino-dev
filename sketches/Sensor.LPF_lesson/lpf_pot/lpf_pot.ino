const byte potPin = A5;
const float K = 1.0;

float val = 0;

void setup() {
  Serial.begin(9600);
  pinMode( potPin, INPUT );
}

void loop() {
  int pot = analogRead( potPin );
  val = val*(1-K) + pot*K;
  Serial.println(val);
  delay(20);
}
