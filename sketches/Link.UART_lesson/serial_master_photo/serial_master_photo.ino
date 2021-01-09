byte photoPin = A0;

void setup() {
    Serial.begin(9600);
    pinMode(photoPin, INPUT);
}

void loop() {
    long val = analogRead(photoPin);
    val = val*255/1023.0;
    Serial.write(val);
    delay(100);
}
