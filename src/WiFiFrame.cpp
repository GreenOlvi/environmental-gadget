#include <SSD1306.h>
#include <ESP8266WiFi.h>
#include "WiFiFrame.h"

WiFiFrame::WiFiFrame(MqttClient* mqtt) {
  _mqtt = mqtt;
}

void WiFiFrame::draw(OLEDDisplay* display) {
  display->setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);

  if (WiFi.isConnected() == false) {
    display->drawString(0, 0, "Not connected");
    return;
  }

  char connStr[30];
  sprintf(connStr, "Connected to %s", WiFi.SSID().c_str());
  display->drawString(0, 0, connStr);
  sprintf(connStr, "%s", WiFi.localIP().toString().c_str());
  display->drawString(0, 10, connStr);
  sprintf(connStr, "%s", WiFi.macAddress().c_str());
  display->drawString(0, 20, connStr);

  display->setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_RIGHT);
  if (_mqtt->isConnected()) {
    display->drawString(128, 0, "mqtt");
  } else {
    display->drawString(128, 0, ":-(");
  }
}