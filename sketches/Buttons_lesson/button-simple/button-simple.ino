const int led = 2;
const int button = 3;
int val = 0;

void setup(){
    pinMode( led, OUTPUT );
    pinMode( button, INPUT );
}

void loop(){
    // записываем в переменную val состояние кнопки 
    val = digitalRead( button );
    // если состояние кнопки - истина, выполняем действие
    if( val == HIGH ){
        // цикл от 0 до 2, с шагом 1
        for( int i=0; i<3; i++ ){
            digitalWrite( led, HIGH );
            delay( 500 );
            digitalWrite( led, LOW );
            delay( 500 );
        }
    }
}
