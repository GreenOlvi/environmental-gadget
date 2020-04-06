#include <Wire.h>
#include "SSD1306.h"
#include "DHTesp.h"

#define DISPLAY_ADDR 0x3c
#define ONE_WIRE_BUS D7

#define M_PI 3.14159265358979323846264338327950288
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)

SSD1306  display(DISPLAY_ADDR, SDA, SCL, GEOMETRY_128_32); //Address set here 0x3C that I found in the scanner, and pins defined as D2 (SDA/Serial Data), and D5 (SCK/Serial Clock).
DHTesp dht;

void setup() {                
  Serial.begin(74880);
  dht.setup(ONE_WIRE_BUS, DHTesp::AM2302);
  display.init();
  display.flipScreenVertically();
}

float t = .0;

void loop() {
  float temperature = dht.getTemperature();
  char tempStr[10];
  sprintf(tempStr, "%.1f°C", temperature);

  float humidity = dht.getHumidity();
  char humStr[4];
  sprintf(humStr, "%.0f%%", humidity);
  
  display.clear();
  display.setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_RIGHT);
  display.drawString(128, 0, tempStr);
  display.drawString(128, 16, humStr);

  int points = 80;
  float data[points];
  fillSin(data, points, t);
  drawGraph(&display, 0, 0, points + 2, 32, data, points);
 
  display.display();

  t += 2.;

  delay(1000);
}

void drawGraph(OLEDDisplay* display, int x, int y, int width, int height, float* data, int dataLength) {
  display->drawRect(x, y, width, height);

  int lastY = height - data[0] * height;
  for (int i = 1; i < dataLength; i++)
  {
    int xp = x + i + 1;
    int yp = y + height - data[i] * height - 1;
    display->drawLine(xp - 1, lastY, xp, yp);
    lastY = yp;
  }
}

void fillSin(float* data, int points, float t) {
  for (int i = 0; i < points; i++) {
    data[i] = (sin(degToRad((i + t)*8))+1)/2;
  }
}