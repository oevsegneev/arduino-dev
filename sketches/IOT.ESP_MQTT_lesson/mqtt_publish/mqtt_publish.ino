#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "X-COM-CITADEL"
#define WLAN_PASS       "id7dq8d!"

#define AIO_SERVER      "192.168.88.248"
#define AIO_SERVERPORT  1883 // use 8883 for SSL
#define AIO_USERNAME    ""
#define AIO_KEY         ""

WiFiClient client;
//WiFiClientSecure client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");

unsigned long pub_next_tm, tm;
unsigned int pub_to = 1000;

//void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

uint32_t x=0;

void loop() {
  MQTT_connect();

  tm = millis();
  if( tm > pub_next_tm ){
    pub_next_tm = tm + pub_to;
    Serial.print(F("\nSending photocell val "));
    Serial.print(x);
    Serial.print("...");
    if (! photocell.publish(x++)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
  }
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
