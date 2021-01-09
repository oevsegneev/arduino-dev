#include <Adafruit_SSD1306.h>

#define WIDTH 128
#define HEIGHT 64
#define PLAYER_WIDTH 5
#define PLAYER_HEIGHT 4
#define CTRL_TO 50

byte pinLeft = A0;
byte pinRight = A1;

int x,y;
unsigned long t, ctrl_next;

byte player[PLAYER_WIDTH*PLAYER_HEIGHT] = {
0,0,1,0,0,
0,1,1,1,0,
0,1,0,1,0,
1,1,0,1,1
};

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
    pinMode( pinLeft, INPUT );
    pinMode( pinRight, INPUT );

    x = WIDTH/2-PLAYER_WIDTH/2;
    y = HEIGHT-1-PLAYER_HEIGHT;
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
        // отрисовка игрока на фрейме
        for(byte px=0; px<PLAYER_WIDTH; px++)
            for(byte py=0; py<PLAYER_HEIGHT; py++)
                display.drawPixel(x+px, y+py, player[px+py*5]);
        // вывод фрейма на дисплей
        display.display();
    }
}
