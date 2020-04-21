#ifndef WiFiFrame_h
#define WiFiFrame_h

#include "SSD1306.h"
#include "Frame.h"

class WiFiFrame : public Frame {
    public:
        WiFiFrame();
        virtual void draw(OLEDDisplay* display);
};

#endif