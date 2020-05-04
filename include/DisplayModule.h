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
        void setCurrentFrame(Frame* frame);
        void displayOn(void);
        void displayOff(void);
        bool isOn(void);
    private:
        SSD1306Wire _display;
        bool _isOn;
        unsigned long nextDisplayUpdate = 0;
        Frame* _currentFrame;
        void drawNoFrame(void);
};

#endif