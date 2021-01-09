//#include <Wire.h>
//#include <SPI.h>
//#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

void setup() {
    Serial.begin(9600);
    bme.begin();  
}

void loop() { 
    // вывод температуры в градусах Цельсия
    Serial.print("t = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    // вывод атмосферного давления в гектопаскалях hPa
    Serial.print("p = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    // вывод высоты над уровнем моря в метрах
    // 1013.25 - давление на уровне моря
    Serial.print("alt = ");
    Serial.print(bme.readAltitude(1013.25));
    Serial.println(" m");

    // вывод влажности в процентах
    Serial.print("h = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();

    delay(1000);
}
