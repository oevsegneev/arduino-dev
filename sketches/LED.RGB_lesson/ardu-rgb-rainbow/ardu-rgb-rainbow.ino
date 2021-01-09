const byte rgbPins[3] = {3,5,6};
const byte rainbow[6][3] = {
  {1,0,0},
  {1,1,0},
  {0,1,0},
  {0,1,1},
  {0,0,1},
  {1,0,1},
};

void setup() {
  for( byte i=0; i<3; i++ )
    pinMode( rgbPins[i], OUTPUT );
}

void loop() {
  for( int i=0; i<6; i++ ){
    for( int k=0; k<3; k++ ){
      digitalWrite( rgbPins[k], rainbow[i][k] );
    }
    delay( 1000 );
  }
}
