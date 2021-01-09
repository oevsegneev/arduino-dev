#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define STATE_PLAY 0
#define STATE_GAMEOVER 1 

#define WIDTH 128 // ширина дисплея
#define HEIGHT 64 // высота дисплея
#define UNIT_WIDTH 5 // ширина юнита
#define UNIT_HEIGHT 4 // высота юнита
#define STATUS_WIDTH 15 // ширина колонки статуса
#define X 0
#define Y 1
#define ENEMY_Y_LIMIT WIDTH-(UNIT_WIDTH+5)*ENEMIES_COLS-STATUS_WIDTH-5

#define CTRL_TO 50
#define ENEMY_TO 1000
#define ROCKET_FLY_TO 50
#define ROCKET_FIRE_TO 1000
#define PLASMA_FLY_TO 50
#define PLASMA_FIRE_TO 1000

#define ENEMIES_COLS 6 // врагов в ряду
#define ENEMIES_ROWS 3 // всего рядов 
#define ENEMIES_Q ENEMIES_COLS*ENEMIES_ROWS // общее количество врагов

struct unit {
    byte state = 1;
    byte coords[2];
};

byte pinLeft = A0;
byte pinRight = A1;

unit player;
unit enemies[ENEMIES_Q];
unit rocket;
unit plasma;
unsigned long t, ctrl_next, enemy_next, rocket_fire_next, rocket_fly_next, plasma_fire_next, plasma_fly_next;

byte enemy_drift_x = 0;
int enemy_drift_dir = 1;
int score;
byte life;
byte state;

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

Adafruit_SSD1306 display(WIDTH, HEIGHT, &Wire, 4);

void enemyMove(){
    if(enemy_drift_dir > 0 && enemy_drift_x < ENEMY_Y_LIMIT){
        enemy_drift_x += 1;
    } else if(enemy_drift_dir < 0 && enemy_drift_x > 5)
        enemy_drift_x -= 1;
    else
        enemy_drift_dir = -1*enemy_drift_dir;

    for(byte e=0; e<ENEMIES_Q; e++)
        enemies[e].coords[X] += enemy_drift_dir;
}

void drawEnemyPlasma(){
    // если статус плазмы - ложь, не отрисовываем её
    if( !plasma.state )
        return;
    display.drawPixel(plasma.coords[X], plasma.coords[Y], 1);
}

void fireEnemyPlasma(byte idx){
    plasma.state = 1;
    plasma.coords[X] = enemies[idx].coords[X]+UNIT_WIDTH/2;
    plasma.coords[Y] = enemies[idx].coords[Y]+UNIT_WIDTH;
}

void plasmaMove(){
    // если статус плазмы - ложь, не обрабатываем её полёт
    if(!plasma.state)
        return;
    // проверяем, пересекает ли плазма контур игрока
    if( plasma.coords[X]>player.coords[X] && 
    plasma.coords[X]<player.coords[X] + UNIT_WIDTH &&
    plasma.coords[Y]>player.coords[Y]){
        // если пересекает, выключаем плазму и запускаем процедуру смерти игрока
        playerKill();
        return;
    }
  
    if( plasma.coords[Y] == HEIGHT ){
        plasma.state = 0;
    }
    plasma.coords[Y] += 1;
}

void enemyKill(byte idx){
    rocket.state = 0;
    enemies[idx].state = 0;
    score ++; // увеличиваем счёт на единицу
}

void handleControls(){
    if( state == STATE_GAMEOVER ){
        if( digitalRead(pinLeft) || digitalRead(pinRight) )
            resetGame();

    } else if( state == STATE_PLAY ){
        if( digitalRead(pinLeft) && player.coords[X]>0 )
            player.coords[X] -= 1;
        else if( digitalRead(pinRight) && player.coords[X] < WIDTH - STATUS_WIDTH - 1 )
            player.coords[X] += 1;
    }
}

void resetGame(){
    // обнуляем счёт игры
    score = 0;
    // даём игроку 3 жизни
    life = 3;
    
    // сбрасываем координаты игрока в начальные значения
    player.coords[X] = WIDTH/2-UNIT_WIDTH/2;
    player.coords[Y] = HEIGHT-UNIT_HEIGHT-1;

    // сбрасываем координаты врагов в начальные значения
    for( byte x=0; x<ENEMIES_COLS; x++){ // в ряду ENEMIES_COLS врагов
        for( byte y=0; y<ENEMIES_ROWS; y++){ // всего ENEMIES_ROWS рядов
            byte e = x + y * ENEMIES_COLS;
            enemies[e].coords[X] = 5 + x*(5+UNIT_WIDTH);
            enemies[e].coords[Y] = 5 + y*(3+UNIT_WIDTH);
            enemies[e].state = 1;
        }
    }

    // устанавливаем состояние игры
    state = STATE_PLAY;

    // выключаем ракету и плазму, если они вдруг где-то летят
    rocket.state = 0;
    plasma.state = 0;
}

void drawInterface(){
    display.setTextSize(1); // размер каждого символа 6*8 пикселей
    display.setTextColor(WHITE); // цвет текста - не черный

    // вывод очков
    display.setCursor(WIDTH - STATUS_WIDTH, 1); // установка курсора в позицию x, y
    display.print(score); // вывод количества очков

    // вывод жизней
    display.setCursor(WIDTH - STATUS_WIDTH, 20); // установка курсора в позицию x, y
    display.print(life); // вывод количества очков
}

void drawGameover(){
    // вывод надписи Game Over
    display.setTextSize(2); // размер каждого символа 12*16 пикселей
    display.setTextColor(WHITE); // цвет текста - не черный
    display.setCursor(100,HEIGHT - 16); // установка курсора в позицию x=100, y=55
    display.setCursor(8, 24);
    display.print("Game Over");

    // вывод текущего счёта
    display.setTextSize(1); // размер каждого символа 6*8 пикселей
    display.setCursor(30, 45);
    display.print("Score:");
    display.setCursor(75, 45);
    display.print(score);
}

void drawPlayer(){
    for(byte px=0; px<UNIT_WIDTH; px++)
        for(byte py=0; py<UNIT_HEIGHT; py++)
            display.drawPixel(player.coords[X]+px, player.coords[Y]+py, player_sprite[px+py*UNIT_WIDTH]);
}

void drawEnemies(){
    for(byte e=0; e<ENEMIES_Q; e++){
        if(enemies[e].state){
            for(byte px=0; px<UNIT_WIDTH; px++){
                for(byte py=0; py<UNIT_HEIGHT; py++)
                    display.drawPixel(enemies[e].coords[X]+px, enemies[e].coords[Y]+py, enemy_sprite[px+py*UNIT_WIDTH]);
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
    for(byte e=0; e<ENEMIES_Q; e++){
        // проверяем, пересекает ли ракета в контур пришельца
        if(enemies[e].state && 
        rocket.coords[X]>enemies[e].coords[X] && 
        rocket.coords[X]<enemies[e].coords[X] + UNIT_WIDTH &&
        rocket.coords[Y]<enemies[e].coords[Y] + UNIT_HEIGHT){
            // если попадает, выключаем ракету и пришельца из игры
            enemyKill(e);
            return;
        }
    }
  
    if( rocket.coords[Y] == 0 ){
        rocket.state = 0;
    }
    rocket.coords[Y] -= 1;
}

void playerKill(){
    plasma.state = 0;
    if( life>0 ){
        life --; // убавляем количество жизней
    } else {
        state = STATE_GAMEOVER;
    }
}

void setup() {
    pinMode( pinLeft, INPUT );
    pinMode( pinRight, INPUT );


    resetGame();

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
        if( state == STATE_GAMEOVER ){
            drawGameover(); // отрисовка экрана Game Over

        } else if( state == STATE_PLAY ){
            drawInterface(); // отрисовка очков и жизней
            drawPlayer(); // отрисовка игрока на фрейме
            drawEnemies(); // отрисовка врагов на фрейме
            drawEnemyPlasma(); // отрисовка плазмы врага
            drawPlayerRocket(); // отрисовка ракеты игрока
        }
        display.display(); // вывод фрейма на дисплей
    }
    if( state == STATE_PLAY && t > enemy_next ){
        enemy_next = t + ENEMY_TO;
        enemyMove();
    }
    if( state == STATE_PLAY && t > plasma_fire_next && !plasma.state ){
        plasma_fire_next = t + PLASMA_FIRE_TO;
        byte e;
        byte idx = 255;
        for(byte y=0; y<ENEMIES_ROWS; y++){
            for(byte x=0; x<ENEMIES_COLS; x++){
                e = x+(2-y)*ENEMIES_COLS;
                // выбираем первого попавшегося живого врага в ближнем к игроку ряду
                if( enemies[e].state ){
                    idx = e;
                    break;
                }
            }
            if( idx < 255 )
                break;
        }
        if( idx < 255 )
            fireEnemyPlasma(idx);
    }
    if( state == STATE_PLAY && t > plasma_fly_next ){
        plasma_fly_next = t + PLASMA_FLY_TO;
        plasmaMove();
    }
    if( state == STATE_PLAY && t > rocket_fire_next && !rocket.state ){
        rocket_fire_next = t + ROCKET_FIRE_TO;
        firePlayerRocket();
    }
    if( state == STATE_PLAY && t > rocket_fly_next ){
        rocket_fly_next = t + ROCKET_FLY_TO;
        rocketMove();
    }
}
