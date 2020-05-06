#include <ESP8266WiFi.h>

#include "Frame.h"
#include "GraphFrame.h"
#include "WiFiFrame.h"
#include "Mqtt.h"
#include "DisplayModule.h"
#include "DataModule.h"
#include "SensorsModule.h"
#include "InputModule.h"
#include "secrets.h"

// #define PUBLISH_MQTT

#define _countof(array) (sizeof(array) / sizeof(array[0]))

#define BUTTON_PIN D6
#define DISPLAY_SLEEP 60 * 1000

#ifdef PUBLISH_MQTT
MqttClient mqtt("envGadget", MQTT_HOST, MQTT_PORT);
#endif

DisplayModule displayModule;
DataModule dataModule;
SensorsModule sensors(&dataModule);
InputModule input(BUTTON_PIN);

const char* ssid = STASSID;
const char* password = STAPSK;

float temperature = NAN;
float humidity = NAN;
float outTemperature = NAN;

TemperatureHumidityGraphFrame insideMinuteTempFrame = TemperatureHumidityGraphFrame(
  dataModule.getTempData()->getMinuteData(),
  dataModule.getTempData()->valueRef(),
  dataModule.getHumidityData()->valueRef());
TemperatureHumidityGraphFrame insideHourTempFrame = TemperatureHumidityGraphFrame(
  dataModule.getTempData()->getHourData(),
  dataModule.getTempData()->valueRef(),
  dataModule.getHumidityData()->valueRef());
TemperatureGraphFrame outsideHourTempFrame = TemperatureGraphFrame(
  dataModule.getAuxTempData()->getHourData(),
  dataModule.getAuxTempData()->valueRef(),
  "out");

#ifdef PUBLISH_MQTT
WiFiFrame wifiFrame = WiFiFrame(&mqtt);
#else
WiFiFrame wifiFrame = WiFiFrame();
#endif

Frame* frames[] = {
  &insideMinuteTempFrame,
  &insideHourTempFrame,
  &outsideHourTempFrame,
  &wifiFrame,
};

const int framesCount = _countof(frames);

Updatable* updatables[] = {
  &sensors,
  &dataModule,
  &input,
#ifdef PUBLISH_MQTT
  &mqtt,
#endif
  &displayModule,
};

const int updatablesCount = _countof(updatables);

int currentFrame = 0;
void onButtonPress(const unsigned long t) {
  if (!displayModule.isOn()) {
    displayModule.displayOn();
  } else {
    currentFrame++;
    if (currentFrame >= framesCount) {
      currentFrame = 0;
    }
    displayModule.setCurrentFrame(frames[currentFrame]);
  }
}

void onButtonRelease(const unsigned long t) {
}

void setup() {                
  sensors.setup();

  input.setup();
  input.onButtonPress(&onButtonPress);
  input.onButtonRelease(&onButtonRelease);

  displayModule.setup();
  displayModule.setCurrentFrame(frames[0]);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}


#ifdef PUBLISH_MQTT
void publish(const char* type, float value) {
  char topic[30];
  sprintf(topic, "env/bedroom/%s", type);
  mqtt.publish(topic, value);
}
#endif

void displaySleepUpdate(const unsigned long t) {
  if (displayModule.isOn() && t - input.lastButtonChange() >= DISPLAY_SLEEP) {
    displayModule.displayOff();
  }
}

unsigned long lastDebugMsg = 0;

void loop() {
  unsigned long time = millis();

  displaySleepUpdate(time);

  for (int i = 0; i < updatablesCount; i++) {
    updatables[i]->update(time);
  }
}