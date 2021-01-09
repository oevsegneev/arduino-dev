#include "Adafruit_MCP23017.h"

const byte ledPin = 0;

Adafruit_MCP23017 mcp;

void setup() {
    mcp.begin(7); // инициализация модуля с адресом 0x07
    mcp.pinMode(ledPin, OUTPUT); // настройка контакта G0 на вывод
}

void loop() {
    mcp.digitalWrite(ledPin, HIGH);
    delay(200);
    mcp.digitalWrite(ledPin, LOW);
    delay(200);
}
