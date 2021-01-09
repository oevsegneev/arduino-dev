#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 6 - LCD chip select (CS)
// pin 7 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);

const unsigned char PROGMEM logoBmp[] = {
  B11111111, B11111111, B10000000,
  B11111111, B11111111, B10000000,
  B11111111, B11111111, B10000000,
  B11111100, B00000011, B10000000,
  B11111000, B00000001, B10000000,
  B11111100, B00000011, B10000000,
  B11111111, B11000011, B10000000,
  B11111111, B10000111, B10000000,
  B11111111, B10001111, B10000000,
  B11111111, B00001111, B10000000,
  B11111110, B00011111, B10000000,
  B11111110, B00011111, B10000000,
  B11111100, B00111111, B10000000,
  B11111100, B01111111, B10000000,
  B11111000, B00000011, B10000000,
  B11111000, B00000001, B10000000
};

void setup() {
  Serial.begin(9600);

  // Инициализация дисплея
  display.begin();

  // Очищаем дисплей
  display.clearDisplay();
  display.display();

  // Устанавливаем контраст
  display.setContrast(50);
  delay(1000);
}


void loop() {
  // Рисуем заранее подготовленное лого
  // Подготовлен массив из 16 пар байтов
  // каждый байт состоит из 8 битов, соответсвенно
  // получаем матрицу 16х16 битов, 1-черный цвет, 0-белый цвет
  display.drawBitmap(LCDWIDTH/2-8, LCDHEIGHT/2-8, logoBmp, 24, 16, BLACK); // x, y, logo, w, h, color 
  display.display();
  delay(10000);
  // Очищаем дисплей
  display.clearDisplay();
  display.display();
  delay(1000);
}
