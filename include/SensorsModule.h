#ifndef SensorsModule_h
#define SensorsModule_h

#include <DHTesp.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Module.h"
#include "DataModule.h"

#define ONE_WIRE_BUS D7
#define OTHER_ONE_WIRE_BUS D5

#define AUX_SENSOR_UPDATE_DELAY 60 * 1000

class SensorsModule : public Module {
    public:
        SensorsModule(DataModule* dataModule);
        void setup(void) override;
        void update(const unsigned long t) override;
    private:
        DataModule* _data;

        DHTesp _dht;
        OneWire _ds;
        DallasTemperature _auxSensors;

        unsigned long _lastDhtUpdate = 0;
        unsigned int _dhtSamplingPeriod;

        unsigned long _lastAuxUpdate = 0;
};

#endif