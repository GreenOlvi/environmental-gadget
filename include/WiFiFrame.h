#ifndef WiFiFrame_h
#define WiFiFrame_h

#include "SSD1306.h"
#include "Frame.h"
#include "Mqtt.h"

class WiFiFrame : public Frame {
    public:
        WiFiFrame(MqttClient* mqtt);
        virtual void draw(OLEDDisplay* display);

    private:
        MqttClient* _mqtt;
};

#endif