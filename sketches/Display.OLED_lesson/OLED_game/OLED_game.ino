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
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();

  delay(1000);
  
}

void loop() {
  for(int x=0; x<111; x++){
    fillrect(x, 0, 16);
    display.display();
  }
}

void fillrect(int x, int y, int s) {
  display.fillRect(x, y, x+s, y+s, WHITE);
}
