int led1pin = 3;

void setup() {
    Serial.begin(9600);

    // настройка контактов
    pinMode(led1pin, OUTPUT);
}

void loop() {
    byte data;
    // есть во входном буфере есть данные
    if( Serial.available() > 0 ){
        data = Serial.read(); // считываем один байт из буфера
        if( data >= 48 && data <= 57 ){
            analogWrite(led1pin, 255*(data-48)/9);
        }
    }
}
