#define SerialTxControl 9   //RS485 управляющий контакт на arduino pin 10
#define RS485Transmit    HIGH
#define RS485Receive     LOW

//char buffer[100];

void setup(void) {
  Serial.begin(9600);
  pinMode(SerialTxControl, OUTPUT);
  digitalWrite(SerialTxControl, RS485Transmit);
}
 
void loop(void) {
  //digitalWrite(SerialTxControl, RS485Transmit);
  Serial.print("ping");  
  //delay(100);
  //digitalWrite(SerialTxControl, RS485Receive);
  delay(500);
}
