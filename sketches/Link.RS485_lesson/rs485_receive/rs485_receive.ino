#define SerialTxControl 9   //RS485 управляющий контакт на arduino pin 10
#define RS485Transmit    HIGH
#define RS485Receive     LOW

char buffer[100];
byte state = 0;

void setup(void) {
  Serial.begin(9600);
  pinMode(13, OUTPUT);  
  pinMode(SerialTxControl, OUTPUT);  
  digitalWrite(SerialTxControl, RS485Receive);   
}
 
void loop(void) {
  int i=0;
  if( Serial.available() ){
    delay(5);
    while( Serial.available() ){
      buffer[i++] = Serial.read();
    }
  }
  if(i>0){
    digitalWrite(13, state);
    state = !state;
    buffer[i++]='\0';
    Serial.println(buffer);
  }
}
