#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#define sclk 13
#define mosi 11
#define cs   10
#define rst  9
#define dc   8

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);  

void setup(void) {
  display.begin();

  uint16_t time = millis();
  display.fillScreen(BLACK);

  lcdTestPattern();
}

void loop() {
}

void lcdTestPattern(void)
{
  uint32_t i,j;
  display.goTo(0, 0);
  
  for(i=0;i<64;i++)
  {
    for(j=0;j<96;j++)
    {
      if(i>55){display.writeData(WHITE>>8);display.writeData(WHITE);}
      else if(i>47){display.writeData(BLUE>>8);display.writeData(BLUE);}
      else if(i>39){display.writeData(GREEN>>8);display.writeData(GREEN);}
      else if(i>31){display.writeData(CYAN>>8);display.writeData(CYAN);}
      else if(i>23){display.writeData(RED>>8);display.writeData(RED);}
      else if(i>15){display.writeData(MAGENTA>>8);display.writeData(MAGENTA);}
      else if(i>7){display.writeData(YELLOW>>8);display.writeData(YELLOW);}
      else {display.writeData(BLACK>>8);display.writeData(BLACK);}
    }
  }
}
