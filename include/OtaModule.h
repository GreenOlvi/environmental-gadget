#ifndef OtaModule_h
#define OtaModule_h

#include <ESP8266WiFi.h>
#include <SPI.h>
#include <ArduinoOTA.h>
#include "Module.h"
#include "DisplayModule.h"

#define OTA_UPDATE_DELAY 1000

class OtaFrame : public Frame {
    public:
        void draw(OLEDDisplay* display) override;
        String text;
        float progress;
};

class OtaModule : public Module {
    public:
        OtaModule(const char* password, DisplayModule* display);
        void setup(void) override;
        void update(const unsigned long t) override;
    private:
        const char* _password;
        unsigned long _lastUpdate;
        bool _isStarted = false;
        DisplayModule* _display = NULL;
        OtaFrame _frame;
        Frame* _prevFrame;
};

#endif