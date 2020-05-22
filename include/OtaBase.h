#ifndef OtaBase_h
#define OtaBase_h

#include <ArduinoOTA.h>
#include "Module.h"

#define OTA_UPDATE_DELAY 1000

class OtaBase : public Module {
    public:
        OtaBase(const char* password);
        void setup(void) override;
        void update(const unsigned long t) override;
    private:
        const char* _password;
        unsigned long _lastUpdate;
        bool _isStarted = false;
};

#endif