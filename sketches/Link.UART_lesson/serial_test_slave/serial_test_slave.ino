void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    Serial.write( Serial.read() );
  }
}
