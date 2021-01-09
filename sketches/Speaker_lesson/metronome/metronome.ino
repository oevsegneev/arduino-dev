const byte dynPin = 2;

void setup() {
  pinMode( dynPin, OUTPUT );
}

void loop() {
  tone( dynPin, 100 );
  delay( 500 );
  noTone();
  delay( 500 );
}
