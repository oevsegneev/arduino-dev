//#define INVERSED

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  #ifdef INVERSED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(150);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(30);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  #else
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(150);
  digitalWrite(LED_BUILTIN, LOW);
  delay(30);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  #endif
}
