#include <Adafruit_SSD1306.h>

#define WIDTH 128
#define HEIGHT 64
#define UNIT_WIDTH 5
#define UNIT_HEIGHT 4
#define X 0
#define Y 1
#define CTRL_TO 50
#define ENEMY_TO 1000

byte pinLeft = A0;
byte pinRight = A1;

byte player_coords[2];
byte enemy_coords[24][2];
unsigned long t, ctrl_next, enemy_next;

byte enemy_drift_x = 0;
int enemy_drift_dir = 1;

// спрайт игрока
byte player_sprite[UNIT_WIDTH*UNIT_HEIGHT] = {
    0,0,1,0,0,
    0,1,1,1,0,
    1,1,0,1,1,
    1,1,0,1,1
};

// спрайт врагов
byte enemy_sprite[UNIT_WIDTH*UNIT_HEIGHT] = {
    1,0,1,0,1,
    0,1,1,1,0,
    0,1,0,1,0,
    1,0,1,0,1
};

Adafruit_SSD1306 display(4);

void enemyMove(){
    if(enemy_drift_dir > 0 && enemy_drift_x < 40)
        enemy_drift_x += 1;  
    else if(enemy_drift_dir < 0 && enemy_drift_x > 5)
        enemy_drift_x -= 1;  
    else
        enemy_drift_dir = -1*enemy_drift_dir;
}

void handleControls(){
    if( digitalRead(pinLeft) && player_coords[X]>0 )
        player_coords[X] -= 1;
    else if( digitalRead(pinRight) && player_coords[X]<WIDTH-1 )
        player_coords[X] += 1;
}

void drawPlayer(){
    for(byte px=0; px<UNIT_WIDTH; px++)
        for(byte py=0; py<UNIT_HEIGHT; py++)
            display.drawPixel(player_coords[X]+px, player_coords[Y]+py, player_sprite[px+py*5]);
}

void drawEnemies(){
    for(byte e=0; e<24; e++)
        for(byte px=0; px<UNIT_WIDTH; px++)
            for(byte py=0; py<UNIT_HEIGHT; py++)
                display.drawPixel(enemy_coords[e][X]+px+enemy_drift_x, enemy_coords[e][Y]+py, enemy_sprite[px+py*5]);
}

void setup() {
    pinMode( pinLeft, INPUT );
    pinMode( pinRight, INPUT );

    // начальные координаты игрока
    player_coords[X] = WIDTH/2-UNIT_WIDTH/2;
    player_coords[Y] = HEIGHT-1-UNIT_HEIGHT;

    // начальные координаты врагов
    for( byte x=0; x<8; x++){ // в ряду 8 врагов
        for( byte y=0; y<3; y++){ // всего 3 ряда врагов
            byte idx = x+y*8;
            enemy_coords[idx][X] = 5 + x*(5+5);
            enemy_coords[idx][Y] = 5 + y*(3+5);
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
        
        handleControls(); // обработка нажатий кнопок

        display.clearDisplay(); // очистка фрейма
        drawPlayer(); // отрисовка игрока на фрейме
        drawEnemies(); // отрисовка врагов на фрейме
        display.display(); // вывод фрейма на дисплей
    }
    if( t > enemy_next ){
        enemy_next = t + ENEMY_TO;
        enemyMove();
    }
}
