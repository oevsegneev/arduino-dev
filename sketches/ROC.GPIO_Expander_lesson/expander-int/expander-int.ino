#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcp;

// к этому контакту Arduino подключен встроенный светодиод
const byte ledPin = 13;

// к этому контакту расширителя подключен внешний светодиод
const byte mcpPin = 0;

// к этому контакту Arduino подключен контакт IA расширителя
const byte arduinoIntPin = 3;

// на 3-м контакте Arduino обслуживается прерывание №1
const byte arduinoInterrupt = 1;

// переменная - флаг, которая примет значение true
// при возникновении прерывания
volatile boolean awakenByInterrupt = false;

void setup(){
  // настраиваем режим контакта для прерывания Arduino
  pinMode(arduinoIntPin,INPUT);

  // инициализируем расширитель с адресом 7
  mcp.begin(7);
  
  // настраиваем прерывания расширителя
  // режим зеркалирования отключен - false
  // режим открытого стока отключен - false
  // инверсия сигнала включена - LOW
  mcp.setupInterrupts(false, false, LOW);

  mcp.pinMode(mcpPin, INPUT); // установка режима
  mcp.pullUp(mcpPin, HIGH); // подтяжка контакта к питанию

  // активируем прерывание расширителя на контакте mcpPin
  // в режиме детектирования спада импульса - FALLING
  mcp.setupInterruptPin(mcpPin, FALLING);

  // настраиваем режим контакта для светодиода
  pinMode(ledPin, OUTPUT);

  // активируем прерывание Arduino под номером arduinoInterrupt
  // с вызовом функции intCallBack
  // в режиме детектирования спада импульса - FALLING
  attachInterrupt(arduinoInterrupt,intCallBack,FALLING);
}

// обработчик прерывания Arduino
void intCallBack(){
  awakenByInterrupt = true;
}

void handleInterrupt(){
  // отключаем прерывание на время пока выполняется обработчик
  detachInterrupt(arduinoInterrupt);

  // мигаем один раз светодиодом
  digitalWrite(ledPin,HIGH);
  delay(100);
  digitalWrite(ledPin,LOW);
  delay(100);

  // ждём, пока кнопку не отпустят
  while( !mcp.digitalRead(mcpPin));

  // сбрасываем флаг нажатия
  awakenByInterrupt = false;

  // включаем прерывание обратно
  attachInterrupt(arduinoInterrupt,intCallBack,FALLING);
}

void loop(){
  // если прерывание сработало и флаг истина
  if(awakenByInterrupt){
      // делаем, что задумали
      handleInterrupt();
  }
}
