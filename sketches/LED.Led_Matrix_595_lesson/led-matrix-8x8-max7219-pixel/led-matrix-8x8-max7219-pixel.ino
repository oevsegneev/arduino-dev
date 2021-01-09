#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 9;
int numberOfHorizontalDisplays = 1; // количество матриц по горизонтали
int numberOfVerticalDisplays = 1; // количество матриц по вертикали

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int wait = 20; // время между

int x,y;

void setup() {
  matrix.setIntensity(4); // яркость от 0 до 15
}

void loop() {
  matrix.drawPixel(x, y, LOW); // гасим пиксел с текущими координатами
  x++;
  if( x==8 ){
    x = 0;
    y++;
    if( y==8 ){
      y = 0;
    }
  }
    
  matrix.drawPixel(x, y, HIGH); // зажигаем пиксел с новыми координатами
  matrix.write(); // вывод всех пикселей на матрицу

  delay(wait);
}
