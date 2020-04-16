#include <Wire.h>
#include <SSD1306.h>
#include <DHTesp.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "WindowedStack.h"
#include "font.h"

#define DEBUG

#define DISPLAY_ADDR 0x3c
#define ONE_WIRE_BUS D7
#define OTHER_ONE_WIRE_BUS D5
#define BUTTON_PIN D6

#define AUX_SENSOR_UPDATE_DELAY 60 * 1000
#define DISPLAY_UPDATE_DELAY 50

SSD1306  display(DISPLAY_ADDR, SDA, SCL, GEOMETRY_128_32);
DHTesp dht;
OneWire ds(OTHER_ONE_WIRE_BUS);
DallasTemperature auxSensors(&ds);

void setup() {                
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  dht.setup(ONE_WIRE_BUS, DHTesp::AM2302);
  auxSensors.begin();
  display.init();
  display.flipScreenVertically();
}

int buttonState = HIGH;
int currentFrame = 0;

float temperature = NAN;
float humidity = NAN;
float outTemperature = NAN;

void loop() {
  long time = millis();
  updateButton(time);
  updateSensors(time);
  updateData(time);
  updateDisplay(&display, time);
}

int lastButtonState = buttonState;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
void updateButton(long t) {
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

void onButtonPress() {
  currentFrame++;
  if (currentFrame > 1) {
    currentFrame = 0;
  }
}

unsigned long nextSensorUpdate = 0;
unsigned long nextAuxSensorUpdate = 0;
void updateSensors(long t) {
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

WindowedStack minuteData = WindowedStack(60);
WindowedStack hourData = WindowedStack(60);

WindowedStack auxMinuteData = WindowedStack(60);
WindowedStack auxHourData = WindowedStack(60);

unsigned long nextMinuteDataUpdate = 0;
unsigned long nextHourDataUpdate = 0;
void updateData(long t) {
  if (t >= nextMinuteDataUpdate) {
    minuteData.push(temperature);
    auxMinuteData.push(outTemperature);
    nextMinuteDataUpdate = t + 1000;
  }

  if (t >= nextHourDataUpdate){
    if (minuteData.Count() > 0) {
      float tempAvg = Average(minuteData.getData(), minuteData.Count());
      hourData.push(tempAvg);
    }

    if (auxMinuteData.Count() > 0) {
      float auxMinuteAvg = Average(auxMinuteData.getData(), auxMinuteData.Count());
      auxHourData.push(auxMinuteAvg);
    }

    nextHourDataUpdate = t + 60 * 1000;
  }
}

unsigned long nextDisplayUpdate = 0;
void updateDisplay(OLEDDisplay* display, long t) {
  if (t >= nextDisplayUpdate) {
    display->clear();
    
    if (currentFrame == 0) {
      char line1[10];
      sprintf(line1, "%.1f°C", temperature);
      char line2[4];
      sprintf(line2, "%.0f%%", humidity);

      drawGraphAnd2Lines(display, &hourData, line1, line2);
    } else if (currentFrame == 1) {
      char line1[10];
      sprintf(line1, "%.1f°C", outTemperature);

      drawGraphAnd2Lines(display, &auxHourData, line1, "out");
    }

    #ifdef DEBUG
    display->setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_LEFT);
    if (buttonState == HIGH) {
      display->drawString(85, 20, "H");
    } else {
      display->drawString(85, 20, "L");
    }

    char frameStr[1];
    sprintf(frameStr, "%d", currentFrame);
    display->drawString(75, 20, frameStr);
    #endif

    display->display();
    nextDisplayUpdate = t + DISPLAY_UPDATE_DELAY;
  }
}

void drawGraphAnd2Lines(OLEDDisplay* display, WindowedStack* graphData, char* line1, char* line2) {
    display->setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_RIGHT);
    display->setFont(ArialMT_Plain_16);

    display->drawString(128, 0, line1);
    display->drawString(128, 16, line2);

    drawGraph(display, 0, 0, 62, 32, graphData->getData(), graphData->Count());
}

void drawGraph(OLEDDisplay* display, int x, int y, int width, int height, float* data, int dataLength) {
  display->drawRect(x, y, width, height);

  int displayedLength = min(width - 2, dataLength);
  int dStart = max(dataLength - displayedLength, 0);
  int xStart = width - 1 - displayedLength;

  float normalizedData[displayedLength];
  float min, max;
  normalize(data + dStart, normalizedData, displayedLength, min, max);

  int lastY = height - normalizedData[0] * height;
  for (int i = 1; i < displayedLength; i++)
  {
    int yp = y + (height - 1) - normalizedData[i] * (height - 2);
    if (lastY != NAN && yp != NAN) {
      int xp = xStart + x + i;
      display->drawLine(xp - 1, lastY, xp, yp);
    }
    lastY = yp;
  }

  char maxStr[10];
  sprintf(maxStr, "%.1f°C", max);

  char minStr[10];
  sprintf(minStr, "%.1f°C", min);

  display->setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_LEFT);
  display->setFont((const uint8_t *)Dialog_plain_8);

  display->drawString(x+1, y, maxStr);
  display->drawString(x+1, y + height - 10, minStr);
}

void normalize(float* data, float* normalized, int n, float &minOut, float &maxOut) {
  float min = INT16_MAX + 0.;
  float max = INT16_MIN + 0.;
  for (int i = 0; i < n; i++) {
    if (min > data[i]) {
      min = data[i];
    }
    if (max < data[i]) {
      max = data[i];
    }
  }

  minOut = min;
  maxOut = max;

  float diff = max - min;

  for (int i = 0; i < n; i++) {
    normalized[i] = (data[i] - min) / diff;
  }
}

float Average(float* data, int dataLength) {
  float sum = 0.;
  int count = 0;
  for (int i = 0; i < dataLength; i++) {
    if (data[i] != NAN) {
      sum += data[i];
      count++;
    }
  }

  if (count > 0) {
    return sum / count;
  } else {
    return NAN;
  }
}