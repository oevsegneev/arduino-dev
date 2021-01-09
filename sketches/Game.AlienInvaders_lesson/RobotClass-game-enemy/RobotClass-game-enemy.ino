#include <Adafruit_SSD1306.h>

#define WIDTH 128
#define HEIGHT 64
#define UNIT_WIDTH 5
#define UNIT_HEIGHT 4
#define X 0
#define Y 1
#define CTRL_TO 50

byte pinLeft = A0;
byte pinRight = A1;

byte player_coords[2];
byte enemy_coords[24][2];
unsigned long t, ctrl_next;

byte player_sprite[UNIT_WIDTH*UNIT_HEIGHT] = {
0,0,1,0,0,
0,1,1,1,0,
0,1,0,1,0,
1,1,0,1,1
};

byte enemy_sprite[UNIT_WIDTH*UNIT_HEIGHT] = {
1,0,1,0,1,
0,1,1,1,0,
0,1,0,1,0,
1,0,1,0,1
};

Adafruit_SSD1306 display(4);

void setup() {
    pinMode( pinLeft, INPUT );
    pinMode( pinRight, INPUT );

    // начальные координаты игрока
    player_coords[X] = WIDTH/2-UNIT_WIDTH/2;
    player_coords[Y] = HEIGHT-1-UNIT_HEIGHT;

    // начальные координаты врагов
    for( byte x=0; x<8; x++){
        for( byte y=0; y<3; y++){
            byte idx = x+y*8;
            enemy_coords[idx][X] = 5 + x*5+x*5;
            enemy_coords[idx][Y] = 5 + y*3+y*5;
        }
    }

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
        if( digitalRead(pinLeft) && player_coords[X]>0 )
            player_coords[X] -= 1;
        if( digitalRead(pinRight) && player_coords[X]<WIDTH-1 )
            player_coords[X] += 1;
        // очистка фрейма
        display.clearDisplay();

        // отрисовка игрока на фрейме
        for(byte px=0; px<UNIT_WIDTH; px++)
            for(byte py=0; py<UNIT_HEIGHT; py++)
                display.drawPixel(player_coords[X]+px, player_coords[Y]+py, player_sprite[px+py*5]);

        // отрисовка врагов на фрейме
        for(byte e=0; e<24; e++)
            for(byte px=0; px<UNIT_WIDTH; px++)
                for(byte py=0; py<UNIT_HEIGHT; py++)
                    display.drawPixel(enemy_coords[e][X]+px, enemy_coords[e][Y]+py, enemy_sprite[px+py*5]);

        // вывод фрейма на дисплей
        display.display();
    }
}
