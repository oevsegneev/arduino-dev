const byte photoPin = 3;

void setup() {
  Serial.begin(9600);
  pinMode(photoPin, INPUT);
}

void loop() {
  byte v = digitalRead(photoPin);
  if(!v){
      Serial.println("received");
  } else {
      Serial.println("-");
  }
  delay(100);
}
