byte ledPin = 13;
byte ledState = 0;
byte v;

unsigned long tm, send_next;
unsigned int send_to = 500; // отправка каждые 500мс

void setup() {
    Serial.begin(9600);
    pinMode( ledPin, OUTPUT );
}

void loop() {
    // проверка наличия данных в буфере UART
    if( Serial.available() ){
        v = Serial.read();
        digitalWrite( ledPin, ledState );
        ledState = !ledState;
    }
    // отправка данных по таймауту
    tm = millis();
    if( tm > send_next ){
        send_next = tm + send_to;
        Serial.write('A');
    }
}
