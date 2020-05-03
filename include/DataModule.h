#ifndef DataModule_h
#define DataModule_h

#include <Arduino.h>
#include "Module.h"
#include "CollectedData.h"

class DataModule : public Module {
    public:
        DataModule();

        void setup();
        void update(unsigned long t) override;

        void setTemp(float temp);
        void setAuxTemp(float temp);
        void setHumidity(float hum);

        float getTemp() const;
        float getAuxTemp() const;
        float getHumidity() const;
    
        CollectedData* getTempData();
        CollectedData* getAuxTempData();
        CollectedData* getHumidityData();

    private:
        void secondUpdate(unsigned long t);
        void minuteUpdate(unsigned long t);

        unsigned long lastSecondUpdate = 0;
        unsigned long lastMinuteUpdate = 0;

        CollectedData _tempData;
        CollectedData _auxTempData;
        CollectedData _humidityData;
};

#endif