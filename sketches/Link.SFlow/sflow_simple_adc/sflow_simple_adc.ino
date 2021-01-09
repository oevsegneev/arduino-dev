const int adc1Pin = A0;
int val = 0;

void setup(){
    Serial.begin(115200);
    pinMode(adc1Pin, INPUT);
}

void loop(){
    val = analogRead(adc1Pin);
    val = ( val/1023.0 )*5.0*1000/10;
    Serial.write(val);
    delay(20);
}
