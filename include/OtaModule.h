#ifndef OtaModule_h
#define OtaModule_h

#include <ESP8266WiFi.h>
#include <SPI.h>
#include <ArduinoOTA.h>
#include "Module.h"

#define OTA_UPDATE_DELAY 1000

class OtaModule : public Module {
    public:
        OtaModule(const char* password);
        void setup(void) override;
        void update(const unsigned long t) override;
    private:
        const char* _password;
        unsigned long _lastUpdate;
        bool _isStarted;
};

#endif