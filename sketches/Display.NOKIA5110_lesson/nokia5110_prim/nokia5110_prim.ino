#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);

void setup()   {
  // Инициализация дисплея
  display.begin();

  // Очищаем дисплей
  display.clearDisplay();
  display.display();

  display.setContrast(50);
  delay(1000);
}


void loop() {
    // пиксел
    display.clearDisplay();
    display.drawPixel(10, 10, BLACK);
    display.display();
    delay(1000);

    // линия
    display.clearDisplay();
    display.drawLine(0, 0, 50, 30, BLACK);
    display.display();
    delay(1000); 

    // прямоугольник
    display.clearDisplay();
    display.drawRect(0, 0, 10, 10, BLACK);
    display.display();
    delay(1000); 

    // прямоугольник залитый
    display.clearDisplay();
    display.fillRect(0, 0, 10, 10, BLACK);
    display.display();
    delay(1000); 

    // треугольник
    display.clearDisplay();
    display.drawTriangle(0, 0, 40, 40, 30, 20, BLACK);
    display.display();
    delay(1000); 

    // окружность в центре
    display.clearDisplay();
    display.drawCircle(display.width()/2, display.height()/2, 10, BLACK);
    display.display();
    delay(1000); 
}

