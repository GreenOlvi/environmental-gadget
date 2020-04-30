#include <DHTesp.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

#include "WindowedStack.h"
#include "Frame.h"
#include "GraphFrame.h"
#include "WiFiFrame.h"
#include "Helpers.h"
#include "Mqtt.h"
#include "DisplayModule.h"
#include "secrets.h"

#define ONE_WIRE_BUS D7
#define OTHER_ONE_WIRE_BUS D5
#define BUTTON_PIN D6

#define AUX_SENSOR_UPDATE_DELAY 60 * 1000

DHTesp dht;
OneWire ds(OTHER_ONE_WIRE_BUS);
DallasTemperature auxSensors(&ds);

MqttClient mqtt("envGadget", MQTT_HOST, MQTT_PORT);

DisplayModule displayModule;

const char* ssid = STASSID;
const char* password = STAPSK;

WindowedStack minuteData = WindowedStack(60);
WindowedStack hourData = WindowedStack(60);

WindowedStack auxMinuteData = WindowedStack(60);
WindowedStack auxHourData = WindowedStack(60);

float temperature = NAN;
float humidity = NAN;
float outTemperature = NAN;

TemperatureHumidityGraphFrame insideMinuteTempFrame = TemperatureHumidityGraphFrame(&minuteData, &temperature, &humidity);
TemperatureHumidityGraphFrame insideHourTempFrame = TemperatureHumidityGraphFrame(&hourData, &temperature, &humidity);
TemperatureGraphFrame outsideHourTempFrame = TemperatureGraphFrame(&auxHourData, &outTemperature, "out");
WiFiFrame wifiFrame = WiFiFrame(&mqtt);

Frame* frames[] = {
  &insideMinuteTempFrame,
  &insideHourTempFrame,
  &outsideHourTempFrame,
  &wifiFrame,
};

const int framesCount = _countof(frames);

Updatable* updatables[] = {
  &mqtt,
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
    nextSensorUpdate = t + dht.getMinimumSamplingPeriod();
  }

  if (t >= nextAuxSensorUpdate) {
    auxSensors.requestTemperatures();
    float tempC = auxSensors.getTempCByIndex(0);
    if (tempC != DEVICE_DISCONNECTED_C) {
      outTemperature = tempC;
    } else {
      outTemperature = NAN;
    }
    nextAuxSensorUpdate = t + AUX_SENSOR_UPDATE_DELAY;
  }
}

void publish(const char* type, float value) {
  char topic[30];
  sprintf(topic, "env/bedroom/%s", type);
  mqtt.publish(topic, value);
}

unsigned long nextMinuteDataUpdate = 0;
unsigned long nextHourDataUpdate = 0;
void updateData(unsigned long t) {
  if (t >= nextMinuteDataUpdate) {
    minuteData.push(temperature);
    auxMinuteData.push(outTemperature);

    nextMinuteDataUpdate = t + 1000;
  }

  if (t >= nextHourDataUpdate){
    if (minuteData.Count() > 0) {
      float avg = average(&minuteData);
      hourData.push(avg);
      publish("temp_in", avg);
    }

    if (auxMinuteData.Count() > 0) {
      float avg = average(&auxMinuteData);
      auxHourData.push(avg);
      publish("temp_out", avg);
    }

    nextHourDataUpdate = t + 60 * 1000;
  }
}

void loop() {
  unsigned long time = millis();

  updateButton(time);
  updateSensors(time);
  updateData(time);

  for (int i = 0; i < updatablesCount; i++) {
    updatables[i]->update(time);
  }
}