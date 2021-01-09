#include <Adafruit_SSD1306.h>

#define WIDTH 128
#define HEIGHT 64
#define UNIT_WIDTH 5
#define UNIT_HEIGHT 4
#define X 0
#define Y 1
#define CTRL_TO 50
#define ENEMY_TO 1000
#define ROCKET_FLY_TO 50
#define ROCKET_START_TO 1000

struct unit {
    byte state = 1;
    byte coords[2];
};

byte pinLeft = A0;
byte pinRight = A1;

unit player;
unit enemies[24];
unit rocket;
unsigned long t, ctrl_next, enemy_next, rocket_fire_next, rocket_fly_next;

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
    if(enemy_drift_dir > 0 && enemy_drift_x < 40){
        enemy_drift_x += 1;
    } else if(enemy_drift_dir < 0 && enemy_drift_x > 5)
        enemy_drift_x -= 1;
    else
        enemy_drift_dir = -1*enemy_drift_dir;

    for(byte e=0; e<24; e++)
        enemies[e].coords[X] += enemy_drift_dir;
}

void handleControls(){
    if( digitalRead(pinLeft) && player.coords[X]>0 )
        player.coords[X] -= 1;
    else if( digitalRead(pinRight) && player.coords[X]<WIDTH-1 )
        player.coords[X] += 1;
}

void drawPlayer(){
    for(byte px=0; px<UNIT_WIDTH; px++)
        for(byte py=0; py<UNIT_HEIGHT; py++)
            display.drawPixel(player.coords[X]+px, player.coords[Y]+py, player_sprite[px+py*5]);
}

void drawEnemies(){
    for(byte e=0; e<24; e++){
        if(enemies[e].state){
            for(byte px=0; px<UNIT_WIDTH; px++){
                for(byte py=0; py<UNIT_HEIGHT; py++)
                    display.drawPixel(enemies[e].coords[X]+px, enemies[e].coords[Y]+py, enemy_sprite[px+py*5]);
            }
        }
    }
}

void drawPlayerRocket(){
    // если статус ракет - ложь, не отрисовываем её
    if( !rocket.state )
        return;
    display.drawPixel(rocket.coords[X], rocket.coords[Y], 1);
}

void firePlayerRocket(){
    rocket.state = 1;
    rocket.coords[X] = player.coords[X]+UNIT_WIDTH/2;
    rocket.coords[Y] = player.coords[Y]-1;
}

void rocketMove(){
    // если статус ракет - ложь, не обрабатываем её полёт
    if(!rocket.state)
        return;
    // перебираем всех врагов
    for(byte e=0; e<24; e++){
        // проверяем, пересекает ли ракета в контур пришельца
        if(enemies[e].state && 
        rocket.coords[X]>enemies[e].coords[X] && 
        rocket.coords[X]<enemies[e].coords[X] + UNIT_WIDTH &&
        rocket.coords[Y]<enemies[e].coords[Y] + UNIT_HEIGHT){
            // если попадает, выключаем ракету и пришельца из игры
            rocket.state = 0;
            enemies[e].state = 0;
            return;
        }
    }
  
    if( rocket.coords[Y] == 0 ){
        rocket.state = 0;
    }
    rocket.coords[Y] -= 1;
}

void setup() {
    pinMode( pinLeft, INPUT );
    pinMode( pinRight, INPUT );

    // начальные координаты игрока
    player.coords[X] = WIDTH/2-UNIT_WIDTH/2;
    player.coords[Y] = HEIGHT-1-UNIT_HEIGHT;

    // начальные координаты врагов
    for( byte x=0; x<8; x++){ // в ряду 8 врагов
        for( byte y=0; y<3; y++){ // всего 3 ряда врагов
            byte e = x+y*8;
            enemies[e].coords[X] = 5 + x*(5+5);
            enemies[e].coords[Y] = 5 + y*(3+5);
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
        drawPlayerRocket(); // отрисовка ракеты игрока
        display.display(); // вывод фрейма на дисплей
    }
    if( t > enemy_next ){
        enemy_next = t + ENEMY_TO;
        enemyMove();
    }
    if( t > rocket_fire_next && !rocket.state ){
        rocket_fire_next = t + ROCKET_START_TO;
        firePlayerRocket();
    }
    if( t > rocket_fly_next ){
        rocket_fly_next = t + ROCKET_FLY_TO;
        rocketMove();
    }
}
