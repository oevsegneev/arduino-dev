#include <Adafruit_SSD1306.h>

#define WIDTH 128
#define HEIGHT 64
#define CTRL_TO 50

byte pinLeft = A0;
byte pinRight = A1;

int x,y;
unsigned long t, ctrl_next;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
    pinMode( pinLeft, INPUT );
    pinMode( pinRight, INPUT );

    x = WIDTH/2;
    y = HEIGHT-1;
    ctrl_next = millis() + CTRL_TO;

    // инициализация дисплея
    display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
    display.clearDisplay();
}

void loop() {
    unsigned long t = millis();
    if( t > ctrl_next ){
        ctrl_next = t + CTRL_TO;
        // обработка нажатий кнопок
        if( digitalRead(pinLeft) && x>0 )
            x = x - 1;
        if( digitalRead(pinRight) && x<WIDTH-1 )
            x = x + 1;
        // очистка фрейма
        display.clearDisplay();
        // отрисовка пиксела на фрейме
        display.drawPixel(x, y, 1);
        // вывод фрейма на дисплей
        display.display();
    }
}
