const int in1 = 4;
const int in2 = 5;
const int en1 = 3;

void setup() {
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(en1, OUTPUT);
}

void loop() {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    for(int i=0; i<256; i++){
      analogWrite(en1, i);
      delay(4);
    }
    for(int i=255; i>=0; i--){
      analogWrite(en1, i);
      delay(4);
    }

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    for(int i=0; i<256; i++){
      analogWrite(en1, i);
      delay(4);
    }
    for(int i=255; i>=0; i--){
      analogWrite(en1, i);
      delay(4);
    }
}
