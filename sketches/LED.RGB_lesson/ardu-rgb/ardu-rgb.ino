const byte rgbPins[3] = {3,5,6};
const byte dim = 1;

void setup() {
  for(byte i=0; i<3; i++){
    pinMode( rgbPins[i], OUTPUT );
  }
  analogWrite(rgbPins[0], 255/dim);
  analogWrite(rgbPins[1], 0);
  analogWrite(rgbPins[2], 0);
}

void loop() {
  for(int i=255; i>=0; i--){
    analogWrite( rgbPins[0], i/dim );
    analogWrite( rgbPins[1], (255-i)/dim );
    delay(10);
  }
  for(int i=255; i>=0; i--){
    analogWrite( rgbPins[1], i/dim );
    analogWrite( rgbPins[2], (255-i)/dim );
    delay(10);
  }
  for(int i=255; i>=0; i--){
    analogWrite( rgbPins[2], i/dim );
    analogWrite( rgbPins[0], (255-i)/dim );
    delay(10);
  }
}
