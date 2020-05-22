#ifndef SensorsModule_h
#define SensorsModule_h

#include <DHTesp.h>

#include "Module.h"
#include "DataModule.h"

#define DHT_PIN D7

class SensorsModule : public Module {
    public:
        SensorsModule(DataModule* dataModule);
        void setup(void) override;
        void update(const unsigned long t) override;
    private:
        DataModule* _data;

        DHTesp _dht;

        unsigned long _lastDhtUpdate = 0;
        unsigned int _dhtSamplingPeriod;
};

#endif