void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);
}

void loop() {
  Serial3.write('A');
  if(Serial3.available()){
    Serial.write( Serial3.read() );
  }
}
