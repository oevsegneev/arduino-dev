#define SERIAL_R 102000 // сопротивление последовательного резистора, 102 кОм

const byte tempPin = A0;

void setup() {
  Serial.begin( 9600 );
  pinMode( tempPin, INPUT );
}

void loop() {
  int t = analogRead( tempPin );
  float tr = 1023.0 / t - 1;
  tr = SERIAL_R / tr;
  Serial.println(tr);
  delay(100);
}
