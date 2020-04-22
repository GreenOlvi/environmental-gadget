#include <SSD1306.h>

#include "Graph.h"
#include "font.h"

void normalize(float* data, float* normalized, int n, float min, float max) {
  float diff = max - min;
  for (int i = 0; i < n; i++) {
    normalized[i] = (data[i] - min) / diff;
  }
}

void findRange(float* data, int n, float &minOut, float &maxOut) {
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
}

void drawGraph(OLEDDisplay* display, int x, int y, int width, int height, float* data, int dataLength) {
  display->drawRect(x, y, width, height);

  int displayedLength = min(width - 2, dataLength);
  int dStart = max(dataLength - displayedLength, 0);
  int xStart = width - 1 - displayedLength;

  float normalizedData[displayedLength];
  float min, max;
  findRange(data + dStart, displayedLength, min, max);

  if (max - min < 1.) {
    float diff = 1. - (max - min);
    min -= diff / 2;
    max += diff / 2;
  }

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