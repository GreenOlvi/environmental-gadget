#include "Wire.h"
#include "SSD1306.h"
#include "DHTesp.h"

#define DISPLAY_ADDR 0x3c
#define ONE_WIRE_BUS D7
#define BUTTON_PIN D6

#define M_PI 3.14159265358979323846264338327950288
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)

SSD1306  display(DISPLAY_ADDR, SDA, SCL, GEOMETRY_128_32);
DHTesp dht;

void setup() {                
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(74880);
  dht.setup(ONE_WIRE_BUS, DHTesp::AM2302);
  display.init();
  display.flipScreenVertically();
}

int buttonState = HIGH;

#define maxPoints 80
int points = 0;
float data[maxPoints];

float temperature = NAN;
float humidity = NAN;

void loop() {
  long time = millis();
  updateButton(time);
  updateSensor(time);
  updateData(time);
  updateDisplay(time);
}

int lastButtonState = buttonState;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
void updateButton(long t) {
  int read = digitalRead(BUTTON_PIN);

  if (read != lastButtonState) {
    lastDebounceTime = t;
  }

  if (t - lastDebounceTime > debounceDelay) {
    if (read != buttonState) {
      buttonState = read;
    }
  }

  lastButtonState = read;
}

unsigned long nextSensorUpdate = 0;
void updateSensor(long t) {
  if (t >= nextSensorUpdate) {
    temperature = dht.getTemperature();
    humidity = dht.getHumidity();

    nextSensorUpdate = t + dht.getMinimumSamplingPeriod();
  }
}

unsigned long nextDataUpdate = 0;
void updateData(long t) {
  if (t >= nextDataUpdate) {
    addDataValue(temperature);
    nextDataUpdate = t + 1000;
  }
}

unsigned long nextDisplayUpdate = 0;
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

    drawGraph(&display, 0, 0, maxPoints + 2, 32, data, points);

    display.setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_LEFT);
    if (buttonState == HIGH) {
      display.drawString(85, 20, "H");
    } else {
      display.drawString(85, 20, "L");
    }
  
    display.display();

    nextDisplayUpdate = t + 50;
  }
}

void addDataValue(float value) {
  if (points < maxPoints) {
    data[points] = value;
    points++;
  } else {
    for (int i = 0; i < points - 1; i++) {
      data[i] = data[i + 1];
    }
    data[points - 1] = value;
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