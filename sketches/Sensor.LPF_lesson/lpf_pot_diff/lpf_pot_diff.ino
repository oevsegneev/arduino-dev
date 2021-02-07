const byte potPin = A5;
const float K = 0.1;

float val = 0;

void setup() {
  Serial.begin(9600);
  pinMode( potPin, INPUT );
}

void loop() {
  int pot = analogRead( potPin );
  val = val*(1-K) + pot*K;
  Serial.print(pot);
  Serial.print(",");
  Serial.println(int(val));
  delay(20);
}
