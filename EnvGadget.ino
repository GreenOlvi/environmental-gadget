#include "Wire.h"
#include "SSD1306.h"
#include "DHTesp.h"

#define DISPLAY_ADDR 0x3c
#define ONE_WIRE_BUS D7

#define M_PI 3.14159265358979323846264338327950288
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)

SSD1306  display(DISPLAY_ADDR, SDA, SCL, GEOMETRY_128_32);
DHTesp dht;

void setup() {                
  Serial.begin(74880);
  dht.setup(ONE_WIRE_BUS, DHTesp::AM2302);
  display.init();
  display.flipScreenVertically();
}


int points = 100;
float data[100];

float temperature = 0.;
float humidity = 0.;

void loop() {
  long time = millis();
  updateSensor(time);
  updateDisplay(time);
}

long nextSensorUpdate = 0;
void updateSensor(long t) {
  if (t >= nextSensorUpdate) {
    temperature = dht.getTemperature();
    humidity = dht.getHumidity();

    nextSensorUpdate = t + dht.getMinimumSamplingPeriod();
  }
}

long nextDisplayUpdate = 0;
void updateDisplay(long t) {
  if (t >= nextDisplayUpdate) {
    char tempStr[10];
    sprintf(tempStr, "%.1f°C", temperature);

    char humStr[4];
    sprintf(humStr, "%.0f%%", humidity);
    
    display.clear();
    display.setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_RIGHT);
    display.drawString(128, 0, tempStr);
    display.drawString(128, 16, humStr);

    fillSin(data, points, t * 0.01);
    drawGraph(&display, 0, 0, 62, 32, data, points);
  
    display.display();

    nextDisplayUpdate = t + 10;
  }
}

void drawGraph(OLEDDisplay* display, int x, int y, int width, int height, float* data, int dataLength) {
  display->drawRect(x, y, width, height);

  int displayedLength = min(width - 2, dataLength);
  int dStart = max(dataLength - displayedLength, 0);

  float normalizedData[displayedLength];
  float min, max;
  normalize(data + dStart, normalizedData, displayedLength, &min, &max);

  int lastY = height - normalizedData[0] * height;
  for (int i = 1; i < displayedLength; i++)
  {
    int xp = x + i + 1;
    int yp = y + (height - 1) - normalizedData[i] * (height - 2);
    display->drawLine(xp - 1, lastY, xp, yp);
    lastY = yp;
  }
}

void normalize(float* data, float* normalized, int n, float* minOut, float* maxOut) {
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

  minOut = &min;
  maxOut = &max;

  float diff = max - min;

  for (int i = 0; i < n; i++) {
    normalized[i] = (data[i] - min) / diff;
  }
}

void fillSin(float* data, int points, float t) {
  for (int i = 0; i < points; i++) {
    data[i] = sin(degToRad((i + t)*8)) + sin(degToRad((i + t)*3));
  }
}