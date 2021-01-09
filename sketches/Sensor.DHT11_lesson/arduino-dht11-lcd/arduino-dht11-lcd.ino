#include "DHT.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define DHTPIN 8     // what pin we're connected to

#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  lcd.begin(16, 2);
  lcd.print("RobotClass.ru"); 
  dht.begin();
}

void loop() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();

  lcd.setCursor(0, 1);
  if (isnan(t) || isnan(h)) {
    lcd.print("Error");
  } else {
    lcd.print("h=");
    lcd.print(h);
    lcd.print("%, t=");
    lcd.print(t);
    lcd.print("C");
  }
  delay(100);
}
