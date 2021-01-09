const byte rgbPins[3] = {3,5,6};

void setup() {
  for( byte i=0; i<3; i++ )
    pinMode( rgbPins[i], OUTPUT );
}

void loop() {
  digitalWrite( rgbPins[2], LOW );
  digitalWrite( rgbPins[0], HIGH );
  delay( 500 );
  digitalWrite( rgbPins[0], LOW );
  digitalWrite( rgbPins[1], HIGH );
  delay( 500 );
  digitalWrite( rgbPins[1], LOW );
  digitalWrite( rgbPins[2], HIGH );
  delay( 500 );
}
