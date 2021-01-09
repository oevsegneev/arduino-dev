byte adcPin = A0;
int val;

void setup() {
    Serial.begin(9600);
    pinMode(adcPin, INPUT);
}

void loop() {
    val = analogRead(adcPin);
    Serial.println(val);
    delay(100);
}
