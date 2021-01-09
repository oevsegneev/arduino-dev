#include <SerialFlow.h>

static const byte PACKET_SIZE = 128;
static const byte VALUE_SIZE = 1;
static const boolean SEPARATE_VALUES = false;

const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
int CLKpin = 4;    // CLK - Clock 
int SIpin = 5;     // SI - Serial input 
int AOpin = A0;   // AO - Analog output
int IntArray[128];
char ptxt[128 + 1];

SerialFlow rd(&Serial);

void setup() {
    rd.setPacketFormat(VALUE_SIZE, PACKET_SIZE, SEPARATE_VALUES);
    rd.begin(115200);

    pinMode(CLKpin, OUTPUT); 
    pinMode(SIpin, OUTPUT);  

    // ускоряем АЦП в 4 раза 
    ADCSRA &= ~PS_128;  
    ADCSRA |= PS_32; // теперь одно АЦП преобразование займет ~30 мкс

    analogReference(DEFAULT);

    // Установка всех GPIO в LOW:
    for( int i=0; i< 14; i++ ){
        digitalWrite(i, LOW);  
    }

    // Запуск первого измерения
    digitalWrite(SIpin, HIGH);
    ClockPulse(); 
    digitalWrite(SIpin, LOW);
  
    // Пока идет измерение, чистим содержимое регистра датчика
    for(int i=0;i< 260;i++){
        ClockPulse(); 
    }
}

void loop(){
    // Запуск нового измерения
    digitalWrite(SIpin, HIGH);
    ClockPulse();
    digitalWrite(SIpin, LOW);

    // Чистка регистра датчика от мусора
    for(int i = 0; i < 128; i++){
        ClockPulse(); 
    }
      
    // Экспозиция 3мс
    delay(3);

    // Запуск сбора данных
    digitalWrite(SIpin, HIGH);
    ClockPulse();
    digitalWrite(SIpin, LOW);

    // Чтение 128 пикселей
    for(int i=0; i < 128; i++){
        delayMicroseconds(20); // пауза для фиксации значения на АЦП
        IntArray[i] = analogRead(AOpin);
        ClockPulse(); 
    }

    // Отправка вектора значений через UART
    for(int i=0; i<128; i++){
        rd.setPacketValue(IntArray[i]/4);
    }
    rd.sendPacket();

    // Здесь можем выполнять основной алгоритм
    // ...
}

// Функция, генерирующая синхроимпульс
void ClockPulse(){
    delayMicroseconds(1);
    digitalWrite(CLKpin, HIGH);
    digitalWrite(CLKpin, LOW);
}
