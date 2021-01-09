#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

int pinCS = 9;
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String tape = "HELLO WORLD!";
int wait = 20;

void setup() {
  matrix.setIntensity(7); // яркость от 0 до 15
}

void loop() {
  for ( int i = 0 ; i < tape.length(); i++ ) {
    matrix.fillScreen(LOW);
    matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
    matrix.write();

    delay(wait);
  }
}

