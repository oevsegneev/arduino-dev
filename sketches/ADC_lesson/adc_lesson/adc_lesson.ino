int val = 0;

void setup() {
    Serial.begin(9600);
    pinMode(A0, INPUT);
}

void loop() {
    val = analogRead(A0);
    Serial.println((5/1024.0)*val);
    delay(1000);
}
