#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>

Max72xxPanel matrix = Max72xxPanel(D8, 4, 1);

unsigned long ticker_next;
String tape = "Hello from RobotClass!";
int spacer = 1;
int width = 5 + spacer;

void setup(void){
  matrix.setIntensity(7);
}

void handleTicker(){
  for ( int i = 0 ; i < width * tape.length() + matrix.width() - 1 - spacer; i++ ) {
    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2;

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < tape.length() ) {
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.write(); // Send bitmap to display
    delay(50);
  }
}

void loop(void){
  handleTicker();
}
