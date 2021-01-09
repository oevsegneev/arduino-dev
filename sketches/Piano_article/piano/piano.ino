#define TONE_TIME 50
#define TONE_DELAY 0

const byte spkPin = 53;

void setup() {
  pinMode( spkPin, OUTPUT );
  //Serial.begin(9600);
}

void loop() {
  //Serial.println(analogRead(A0));
  if( analogRead(A0)<1000 ){
    tone(spkPin,261,TONE_TIME);
    delay(TONE_DELAY);
  }
  if( analogRead(A1)<1000 ){
    tone(spkPin,294,TONE_TIME);
    delay(TONE_DELAY);
  }
  if( analogRead(A2)<1000 ){
    tone(spkPin,329,TONE_TIME);
    delay(TONE_DELAY);
  }
  if( analogRead(A3)<1000 ){
    tone(spkPin,349,TONE_TIME);
    delay(TONE_DELAY);
  }
  if( analogRead(A4)<1000 ){
    tone(spkPin,392,TONE_TIME);
    delay(TONE_DELAY);
  }
  if( analogRead(A5)<1000 ){
    tone(spkPin,440,TONE_TIME);
    delay(TONE_DELAY);
  }
  if( analogRead(A6)<1000 ){
    tone(spkPin,493,TONE_TIME);
    delay(TONE_DELAY);
  }
  if( analogRead(A7)<1000 ){
    tone(spkPin,523,TONE_TIME);
    delay(TONE_DELAY);
  }
}
