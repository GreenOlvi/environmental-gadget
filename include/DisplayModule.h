#ifndef DisplayModule_h
#define DisplayModule_h

#include <Arduino.h>
#include <Wire.h>
#include <SSD1306Wire.h>
#include "Updatable.h"
#include "Frame.h"

#define DISPLAY_ADDR 0x3c
#define DISPLAY_UPDATE_DELAY 50

class DisplayModule : public Updatable {
    public:
        DisplayModule(void);
        void setup(void);
        void update(unsigned long t);
        OLEDDisplay* getDisplay(void);
        void setCurrentFrame(Frame* frame);
    private:
        SSD1306Wire _display;
        unsigned long nextDisplayUpdate = 0;
        Frame* _currentFrame;
        void drawNoFrame();
};

#endif