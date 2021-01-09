int led1pin = 2;
int led2pin = 3;
int led3pin = 4;
int led4pin = 5;

void setup() {
    Serial.begin(9600);

    // настройка контактов
    pinMode(led1pin, OUTPUT);
    pinMode(led2pin, OUTPUT);
    pinMode(led3pin, OUTPUT);
    pinMode(led4pin, OUTPUT);
}

void loop() {
    char data;
    // есть во входном буфере есть данные
    if( Serial.available() > 0 ){
        data = Serial.read(); // считываем один байт из буфера
        switch(data){
        case 'R':{
            digitalWrite(led1pin, HIGH);
            break;  
        }
        case 'L':{
            digitalWrite(led2pin, HIGH);
            break;  
        }
        case 'F':{
            digitalWrite(led3pin, HIGH);
            break;  
        }
        case 'B':{
            digitalWrite(led4pin, HIGH);
            break;  
        }
        default:{
            digitalWrite(led1pin, LOW);
            digitalWrite(led2pin, LOW);
            digitalWrite(led3pin, LOW);
            digitalWrite(led4pin, LOW);
        }
        }
    }
}
