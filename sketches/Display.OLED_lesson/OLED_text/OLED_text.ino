#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
//Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
Adafruit_SSD1306 display(4);

void setup() {
    // инициализация и очистка дисплея
    display.begin(SSD1306_SWITCHCAPVCC, 0x3d);
    display.clearDisplay();
    display.display();
   
    delay(1000);

    display.setTextSize(1);  // установка размера шрифта
    display.setTextColor(WHITE); // установка цвета текста
    display.setCursor(0,0); // установка курсора
  
    display.println("Hello, world!");
    display.display();
}

void loop() {
}
