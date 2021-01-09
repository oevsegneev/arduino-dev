#define SerialTxControl 9   //RS485 управляющий контакт на arduino pin 10
#define RS485Transmit    HIGH
#define RS485Receive     LOW

char buffer[100];
byte state = 0;

void setup(void) {
  Serial.begin(115200);
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
    if(i>0){
      buffer[i++]='\0';
      if( !strcmp(buffer, "ping") ){
        digitalWrite(13, state);
        state = !state;
        digitalWrite(SerialTxControl, RS485Transmit);
        Serial.print("pong");  
        delay(10);
        digitalWrite(SerialTxControl, RS485Receive);
      }
    }
  }
}
