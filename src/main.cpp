#include <ESP8266WiFi.h>

#include "Mqtt.h"
#include "DataModule.h"
#include "SensorsModule.h"
#include "InputModule.h"
#include "OtaBase.h"
#include "secrets.h"

#define PUBLISH_MQTT

#define _countof(array) (sizeof(array) / sizeof(array[0]))

#ifdef PUBLISH_MQTT
MqttClient mqtt("envGadget", MQTT_HOST, MQTT_PORT);
#endif

DataModule dataModule;
SensorsModule sensors(&dataModule);
OtaBase ota(OTA_PASSWORD);

const char* ssid = STASSID;
const char* password = STAPSK;

float temperature = NAN;
float humidity = NAN;

Updatable* updatables[] = {
  &sensors,
  &dataModule,
#ifdef PUBLISH_MQTT
  &mqtt,
#endif
  &ota,
};

const int updatablesCount = _countof(updatables);

void setup() {                
  sensors.setup();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  ota.setup();

  mqtt.setup();
}


#ifdef PUBLISH_MQTT
void publish(const char* type, float value) {
  char topic[30];
  sprintf(topic, "env/%s/%s", DEVICE_NAME, type);
  mqtt.publish(topic, value);
}

unsigned long nextMqttPublish = 0;
void mqttPublishUpdate(const unsigned long t) {
  if (t >= nextMqttPublish) {
    publish("temp_in", dataModule.getTemp());
    publish("hum_in", dataModule.getHumidity());
    nextMqttPublish = t + 60 * 1000;
  }
}
#endif

void loop() {
  unsigned long time = millis();

  mqttPublishUpdate(time);

  for (int i = 0; i < updatablesCount; i++) {
    updatables[i]->update(time);
  }
}