#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 10;
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

const byte data[8] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
};

void setup() {
  matrix.setIntensity(7); // уровень яркости от 0 до 15

  matrix.fillScreen(LOW); // очистка матрицы
  for ( int y = 0; y < 8; y++ ) {
    for ( int x = 0; x < 8; x++ ) {
      matrix.drawPixel(x, y, data[y] & (1<<x));
    }
  }
  matrix.write();
}

void loop() {
}

