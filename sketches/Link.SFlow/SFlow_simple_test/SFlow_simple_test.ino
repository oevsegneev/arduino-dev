void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  
  
  
  double t = (analogRead(A0)/1024.0)*255;
  
  if(t > 170)
  {
    analogWrite(3, 0);
  }
  else
    analogWrite(3, 255);
  
  Serial.write(byte(t));
  delay(10);
}
