int m1FPin = 2;
int m1BPin = 3;
int m2FPin = 5;
int m2BPin = 6;

int m1PWM = 9;
int m2PWM = 10;

int sensPin = 11;
int ledPin = 12;
int buzzPin = 13;

int sens_raw;

const byte DIR_FORWARD = 0; 
const byte DIR_BACKWARD = 1; 
const byte DIR_LEFT = 2; 
const byte DIR_RIGHT = 3; 
const byte DIR_STOP = 4; 

void blink( byte iter, int time ){
    for( byte i=0; i<iter; i++ ){
        digitalWrite( ledPin, HIGH );
        digitalWrite( buzzPin, HIGH );
        delay(time);
        digitalWrite( ledPin, LOW );
        digitalWrite( buzzPin, LOW );
        delay(time);
    }
}

void initMotors(){
    pinMode(m1PWM, OUTPUT);
    pinMode(m2PWM, OUTPUT);

    pinMode(m1FPin, OUTPUT);
    pinMode(m1BPin, OUTPUT);
    pinMode(m2FPin, OUTPUT);
    pinMode(m2BPin, OUTPUT);

    analogWrite( m1PWM, 255 );
    analogWrite( m2PWM, 255 );

    walkDir( DIR_FORWARD );
}

void walkDir( byte dir ){
    if( dir==DIR_FORWARD ){
        digitalWrite( m1FPin, LOW );
        digitalWrite( m1BPin, HIGH );
        digitalWrite( m2FPin, LOW );
        digitalWrite( m2BPin, HIGH );
    }
    else if( dir==DIR_LEFT ){
        digitalWrite( m1FPin, HIGH );
        digitalWrite( m1BPin, LOW );
        digitalWrite( m2FPin, LOW );
        digitalWrite( m2BPin, HIGH );
    }
    else if( dir==DIR_STOP ){
        digitalWrite( m1FPin, LOW );
        digitalWrite( m1BPin, LOW );
        digitalWrite( m2FPin, LOW );
        digitalWrite( m2BPin, LOW );
    }
}

void setup()  {
    pinMode(ledPin, OUTPUT);
    pinMode(buzzPin, OUTPUT);

    blink(5,50);

    initMotors();    
//   Serial.begin(9600);
} 

void loop()  {
    sens_raw = digitalRead( sensPin ); 

    if( sens_raw )
        walkDir( DIR_FORWARD );
    else {
        walkDir( DIR_LEFT );
        blink(1,50);
    }
//    Serial.print(sens_raw);
//    delay(500);
}


