const int led = 2;
const int button = 3;
int val = 0;
byte state = 0; // переменная состояния

void setup(){
    pinMode( led, OUTPUT );
    pinMode( button, INPUT );
}

void loop(){
    // записываем в переменную val состояние кнопки 
    val = digitalRead( button );
    // если состояние кнопки - истина, выполняем действие
    if( val == HIGH ){
         // меняем состояние на противоположное
        state = !state;
        if( state == HIGH ){
            // если текущее состояние - истина, зажигаем светодиод
            digitalWrite( led, HIGH );
        } else {
            // если текущее состояние - ложь, гасим светодиод
            digitalWrite( led, LOW );
        }
        delay( 300 );
    }
}
