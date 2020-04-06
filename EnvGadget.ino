#include <Wire.h>  
#include "SSD1306.h"
#include "DHTesp.h"

#define DISPLAY_ADDR 0x3c
#define ONE_WIRE_BUS D7

SSD1306  display(DISPLAY_ADDR, SDA, SCL, GEOMETRY_128_32); //Address set here 0x3C that I found in the scanner, and pins defined as D2 (SDA/Serial Data), and D5 (SCK/Serial Clock).
DHTesp dht;

void setup()
{                
  Serial.begin(74880);
  dht.setup(ONE_WIRE_BUS, DHTesp::AM2302);
  display.init();
  display.flipScreenVertically();
}

void loop()
{
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  char tempStr[20];
  sprintf(tempStr, "%f C, %f%%", temperature, humidity);

  display.clear();
  display.drawString(0, 0, tempStr);
  display.display();

  delay(2000);
}