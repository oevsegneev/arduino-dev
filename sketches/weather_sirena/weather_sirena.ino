unsigned long int LED_TIMEOUT_MS = 2000;
unsigned long int SIR_TIMEOUT_MS = 10000;

unsigned long int time_led = 0;
unsigned long int time_sir = 0;

int led_pin = 7;
int sir_pin = 6;

void setup() {
    pinMode(led_pin, OUTPUT);
    pinMode(sir_pin, OUTPUT);

//7200
    digitalWrite(sir_pin, LOW);
    digitalWrite(led_pin, LOW);
    for( long i=0; i<7200; i++ ){
      delay(1000);
    }
}

void loop() {
    if( millis()>(time_led+LED_TIMEOUT_MS) ){
        time_led = millis();
        digitalWrite(led_pin, HIGH);
        delay(500);
        digitalWrite(led_pin, LOW);
        delay(500);
    }
    if( millis()>time_sir+SIR_TIMEOUT_MS ){
        time_sir = millis();
        digitalWrite(sir_pin, HIGH);
        delay(1000);
        digitalWrite(sir_pin, LOW);
        delay(500);
    }
}
