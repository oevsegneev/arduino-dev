int in1pin = 2;
int in2pin = 4;
int in3pin = 6;
int in4pin = 7;

// поворт направо
void turnRight(){
    digitalWrite(in1pin, HIGH);
    digitalWrite(in2pin, LOW);
    digitalWrite(in3pin, LOW);
    digitalWrite(in4pin, HIGH);
}

// поворт налево
void turnLeft(){
    digitalWrite(in1pin, LOW);
    digitalWrite(in2pin, HIGH);
    digitalWrite(in3pin, HIGH);
    digitalWrite(in4pin, LOW);
}

// движение назад
void backward(){
    digitalWrite(in1pin, HIGH);
    digitalWrite(in2pin, LOW);
    digitalWrite(in3pin, HIGH);
    digitalWrite(in4pin, LOW);
}

// движение вперед
void forward(){
    digitalWrite(in1pin, LOW);
    digitalWrite(in2pin, HIGH);
    digitalWrite(in3pin, LOW);
    digitalWrite(in4pin, HIGH);
}

// остановка
void mstop(){
    digitalWrite(in1pin, LOW);
    digitalWrite(in2pin, LOW);
    digitalWrite(in3pin, LOW);
    digitalWrite(in4pin, LOW);
}

void setup() {
    Serial.begin(9600);

    // настройка контактов
    pinMode(in1pin, OUTPUT);
    pinMode(in2pin, OUTPUT);
    pinMode(in3pin, OUTPUT);
    pinMode(in4pin, OUTPUT);
}

void loop() {
    byte data;
    // есть во входном буфере есть данные
    if( Serial.available() > 0 ){
        data = Serial.read(); // считываем один байт из буфера
        switch(data){
        case 'R':{
            turnRight();
            break;  
        }
        case 'L':{
            turnLeft();
            break;  
        }
        case 'F':{
            forward();
            break;  
        }
        case 'B':{
            backward();
            break;  
        }
        default:{
            mstop();
        }
        }
    }
}
