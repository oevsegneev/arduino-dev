const int ledPin = 2;

int next_time;
int timeout = 500; // половина периода мигания
int led_state = 0; // начальное состояние светодиода - выключен

void setup() {
    pinMode(ledPin, OUTPUT);

    digitalWrite(ledPin, led_state); // гасим светодиод
    next_time = millis() + timeout; // вычисляем время следующего переключения
}

void loop() {
    int now_time = millis(); // текущее время
    if( now_time >= next_time ){ // если текущее время превысило намеченное время, то
        next_time = now_time + timeout; // вычисляем время следующего переключения
        led_state = !led_state; // меняем состояние на противоположное
        digitalWrite(ledPin, led_state); // зажигаем или гасим светодиод
    }
}
