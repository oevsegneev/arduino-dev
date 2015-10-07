void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);

}

void loop() {
  Serial.write(byte((analogRead(A0)/1024.0)*255));
  delay(10);
}
