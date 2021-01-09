#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define WLAN_SSID       "makeitlab"
#define WLAN_PASS       "noumen2001"

#define AIO_SERVER      "45.86.181.43"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "puser"
#define AIO_KEY         "dEr40n"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, "bmp280/temp");

Adafruit_BMP280 bme;

unsigned long pub_next_tm, tm;
unsigned int pub_to = 5000;

void setup() {
  Serial.begin(9600);
  
  // инициализация BMP280
  if (!bme.begin()) {  
    while (1);
  }

  // инициализация WIFI
  delay(10);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("connected to wifi");
}

void loop() {
  tm = millis();
  if( tm > pub_next_tm ){
    pub_next_tm = tm + pub_to;
    MQTT_connect(); // подключаемся к брокеру
    float t = bme.readTemperature(); // считываем температуру
    Serial.print("t = ");
    Serial.println(t);
    temperature.publish(t); // публикуем mqtt сообщение
  }
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
       mqtt.disconnect();
       delay(5000);
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("connected to mqtt");
}
