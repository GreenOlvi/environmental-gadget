#ifndef DisplayModule_h
#define DisplayModule_h

#include <Arduino.h>
#include <Wire.h>
#include <SSD1306Wire.h>
#include "Module.h"
#include "Frame.h"

#define DISPLAY_ADDR 0x3c
#define DISPLAY_UPDATE_DELAY 50

class DisplayModule : public Module {
    public:
        DisplayModule(void);
        void setup(void) override;
        void update(const unsigned long t) override;
        OLEDDisplay* getDisplay(void);
        void setCurrentFrame(Frame* frame);
    private:
        SSD1306Wire _display;
        unsigned long nextDisplayUpdate = 0;
        Frame* _currentFrame;
        void drawNoFrame();
};

#endif