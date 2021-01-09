const byte rPin = 3;
const byte gPin = 5;
const byte bPin = 6;

void setup() {
  pinMode( rPin, OUTPUT );
  pinMode( gPin, OUTPUT );
  pinMode( bPin, OUTPUT );
}

void loop() {
  digitalWrite( bPin, LOW );
  digitalWrite( rPin, HIGH );
  delay( 1000 );
  digitalWrite( rPin, LOW );
  digitalWrite( gPin, HIGH );
  delay( 1000 );
  digitalWrite( gPin, LOW );
  digitalWrite( bPin, HIGH );
  delay( 1000 );
}
