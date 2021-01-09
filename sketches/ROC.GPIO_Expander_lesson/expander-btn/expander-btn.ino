#include "Adafruit_MCP23017.h"

const byte ledPin = 0;
const byte btnPin = 1;

Adafruit_MCP23017 mcp;

void setup() {
    mcp.begin(7); // инициализация модуля с адресом 0x07
    mcp.pinMode(ledPin, OUTPUT); // настройка контакта G0 на вывод
    mcp.pinMode(btnPin, INPUT); // настройка контакта G1 на ввод
    mcp.pullUp(btnPin, HIGH); // подтяжка контакта G1 к питанию
}

void loop() {
    if(mcp.digitalRead(btnPin)==LOW) // если кнопка нажата, то
        mcp.digitalWrite(ledPin, HIGH); // зажигаем светодиод
    else // иначе
        mcp.digitalWrite(ledPin, LOW); // гасим светодиод
}
