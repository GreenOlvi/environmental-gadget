#include <DHTesp.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

#include "Frame.h"
#include "GraphFrame.h"
#include "WiFiFrame.h"
#include "Mqtt.h"
#include "DisplayModule.h"
#include "DataModule.h"
#include "secrets.h"

// #define PUBLISH_MQTT

#define _countof(array) (sizeof(array) / sizeof(array[0]))

#define ONE_WIRE_BUS D7
#define OTHER_ONE_WIRE_BUS D5
#define BUTTON_PIN D6

#define AUX_SENSOR_UPDATE_DELAY 60 * 1000

DHTesp dht;
OneWire ds(OTHER_ONE_WIRE_BUS);
DallasTemperature auxSensors(&ds);

#ifdef PUBLISH_MQTT
MqttClient mqtt("envGadget", MQTT_HOST, MQTT_PORT);
#endif

DisplayModule displayModule;
DataModule dataModule;

const char* ssid = STASSID;
const char* password = STAPSK;

float temperature = NAN;
float humidity = NAN;
float outTemperature = NAN;

TemperatureHumidityGraphFrame insideMinuteTempFrame = TemperatureHumidityGraphFrame(dataModule.getTempData()->getMinuteData(), &temperature, &humidity);
TemperatureHumidityGraphFrame insideHourTempFrame = TemperatureHumidityGraphFrame(dataModule.getTempData()->getHourData(), &temperature, &humidity);
TemperatureGraphFrame outsideHourTempFrame = TemperatureGraphFrame(dataModule.getAuxTempData()->getHourData(), &outTemperature, "out");

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
  &dataModule,
#ifdef PUBLISH_MQTT
  &mqtt,
#endif
  &displayModule,
};

const int updatablesCount = _countof(updatables);

void setup() {                
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  dht.setup(ONE_WIRE_BUS, DHTesp::AM2302);
  auxSensors.begin();
  displayModule.setup();
  displayModule.setCurrentFrame(frames[0]);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}

int buttonState = HIGH;
int currentFrame = 0;

void onButtonPress() {
  currentFrame++;
  if (currentFrame >= framesCount) {
    currentFrame = 0;
  }
  displayModule.setCurrentFrame(frames[currentFrame]);
}

int lastButtonState = buttonState;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
void updateButton(unsigned long t) {
  int read = digitalRead(BUTTON_PIN);

  if (read != lastButtonState) {
    lastDebounceTime = t;
    lastButtonState = read;
  }

  if (t - lastDebounceTime > debounceDelay) {
    if (read != buttonState) {
      buttonState = read;
      if (buttonState == LOW) {
        onButtonPress();
      }
    }
  }
}

unsigned long nextSensorUpdate = 0;
unsigned long nextAuxSensorUpdate = 0;
void updateSensors(unsigned long t) {
  if (t >= nextSensorUpdate) {
    temperature = dht.getTemperature();
    humidity = dht.getHumidity();

    dataModule.setTemp(temperature);
    dataModule.setHumidity(humidity);

    nextSensorUpdate = t + dht.getMinimumSamplingPeriod();
  }

  if (t >= nextAuxSensorUpdate) {
    auxSensors.requestTemperatures();
    float tempC = auxSensors.getTempCByIndex(0);
    if (tempC != DEVICE_DISCONNECTED_C) {
      outTemperature = tempC;
      dataModule.setAuxTemp(tempC);
    } else {
      outTemperature = NAN;
      dataModule.setAuxTemp(NAN);
    }
    nextAuxSensorUpdate = t + AUX_SENSOR_UPDATE_DELAY;
  }
}

#ifdef PUBLISH_MQTT
void publish(const char* type, float value) {
  char topic[30];
  sprintf(topic, "env/bedroom/%s", type);
  mqtt.publish(topic, value);
}
#endif

void loop() {
  unsigned long time = millis();

  updateButton(time);
  updateSensors(time);

  for (int i = 0; i < updatablesCount; i++) {
    updatables[i]->update(time);
  }
}