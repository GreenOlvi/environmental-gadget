#include <ESP8266WiFi.h>

#include "Frame.h"
#include "GraphFrame.h"
#include "WiFiFrame.h"
#include "Mqtt.h"
#include "DisplayModule.h"
#include "DataModule.h"
#include "SensorsModule.h"
#include "secrets.h"

// #define PUBLISH_MQTT

#define _countof(array) (sizeof(array) / sizeof(array[0]))

#define BUTTON_PIN D6
#define DISPLAY_SLEEP 10 * 1000

#ifdef PUBLISH_MQTT
MqttClient mqtt("envGadget", MQTT_HOST, MQTT_PORT);
#endif

DisplayModule displayModule;
DataModule dataModule;
SensorsModule sensors(&dataModule);

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
#ifdef PUBLISH_MQTT
  &mqtt,
#endif
  &displayModule,
};

const int updatablesCount = _countof(updatables);

void setup() {                
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  sensors.setup();
  displayModule.setup();
  displayModule.setCurrentFrame(frames[0]);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}

int buttonState = HIGH;
int currentFrame = 0;
unsigned long lastButtonInput = 0;

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

int lastButtonState = buttonState;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
void updateButton(const unsigned long t) {
  int read = digitalRead(BUTTON_PIN);

  if (read != lastButtonState) {
    lastDebounceTime = t;
    lastButtonState = read;
  }

  if (t - lastDebounceTime > debounceDelay) {
    if (read != buttonState) {
      buttonState = read;
      lastButtonInput = t;
      if (buttonState == LOW) {
        onButtonPress(t);
      } else {
        onButtonRelease(t);
      }
    }
  }
}

#ifdef PUBLISH_MQTT
void publish(const char* type, float value) {
  char topic[30];
  sprintf(topic, "env/bedroom/%s", type);
  mqtt.publish(topic, value);
}
#endif

void displaySleepUpdate(const unsigned long t) {
  if (displayModule.isOn() && t - lastButtonInput >= DISPLAY_SLEEP) {
    displayModule.displayOff();
  }
}

void loop() {
  unsigned long time = millis();

  updateButton(time);
  displaySleepUpdate(time);

  for (int i = 0; i < updatablesCount; i++) {
    updatables[i]->update(time);
  }
}