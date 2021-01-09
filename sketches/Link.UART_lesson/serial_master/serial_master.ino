void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.write('A');
    delay(500);
}
