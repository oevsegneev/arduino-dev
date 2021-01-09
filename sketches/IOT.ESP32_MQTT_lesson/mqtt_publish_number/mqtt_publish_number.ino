#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define WLAN_SSID       "makeitlab"
#define WLAN_PASS       "noumen2001"

#define AIO_SERVER      "mqtt.robotclass.ru"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "fred"
#define AIO_KEY         "123"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish counter = Adafruit_MQTT_Publish(&mqtt, "counter");

unsigned long pub_next_tm, tm;
unsigned int pub_to = 5000;
unsigned int cnt = 0;

void setup() {
  Serial.begin(9600);
  
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
    Serial.println(cnt);
    counter.publish(cnt); // публикуем mqtt сообщение
    cnt++;
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
