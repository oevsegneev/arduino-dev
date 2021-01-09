//#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);

void setup() {
  // инициализация и очистка дисплея
  display.begin();
  display.clearDisplay();
  display.display();

  // установка контраста
  display.setContrast(20);
  delay(1000);
  
  display.setTextSize(1);  // установка размера шрифта
  display.setTextColor(BLACK); // установка цвета текста
  display.setCursor(0,0); // установка цвета текста
  
  display.println("Hello, world!");
  display.display();
}

void loop() {
}

