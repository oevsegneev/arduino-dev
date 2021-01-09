const int dynPin = 2;
 
int numTones = 10;
// Ноты C,C#,D,D#,E,F,F#,G,G#,A
int tones[10] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};

void setup(){
  pinMode( dynPin, OUTPUT );
}
 
void loop(){
  for( int i = 0; i < numTones; i++ ){
    tone( dynPin, tones[i] );
    delay( 500 );
  }
  noTone( dynPin );
}
