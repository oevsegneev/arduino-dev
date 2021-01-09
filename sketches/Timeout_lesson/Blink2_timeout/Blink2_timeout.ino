const int ledPin_1 = 2;
const int ledPin_2 = 3;

int next_time_1; // время очередного переключения первого светодиода
int next_time_2; // ... второго светодиода
int timeout_1 = 500; // половина периода мигания первого светодиода
int timeout_2 = 150; // ... второго светодиода
int led_state_1 = 0; // начальное состояние первого светодиода - выключен
int led_state_2 = 0; // ... второго светодиода

void setup() {
    pinMode(ledPin_1, OUTPUT);
    pinMode(ledPin_2, OUTPUT);

    digitalWrite(ledPin_1, led_state_1); // гасим первый светодиод
    digitalWrite(ledPin_2, led_state_2); // гасим второй светодиод

    next_time_1 = millis() + timeout_1; // вычисляем время следующего переключения первого светодиода
    next_time_2 = millis() + timeout_2; // ... второго светодиода
}

void loop() {
    int now_time = millis(); // текущее время
    if( now_time >= next_time_1 ){ // если текущее время превысило намеченное время, то
        next_time_1 = now_time + timeout_1; // вычисляем время следующего переключения
        led_state_1 = !led_state_1; // меняем состояние на противоположное
        digitalWrite(ledPin_1, led_state_1); // зажигаем или гасим светодиод
    }

    now_time = millis();
    if( now_time >= next_time_2 ){
        next_time_2 = now_time + timeout_2;
        led_state_2 = !led_state_2;
        digitalWrite(ledPin_2, led_state_2);
    }
}
