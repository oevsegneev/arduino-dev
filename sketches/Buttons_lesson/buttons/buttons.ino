const byte btn1 = 2;
const byte btn2 = 4;

void setup() {
  Serial.begin(9600);
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
}

void loop() {
  Serial.print(digitalRead(btn1));
  Serial.print(" - ");
  Serial.print(digitalRead(btn2));
  Serial.println();
  delay(50);
}
