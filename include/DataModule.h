#ifndef DataModule_h
#define DataModule_h

#include <Arduino.h>
#include "Module.h"
#include "CollectedData.h"

class DataModule : public Module {
    public:
        DataModule();

        void setup() override;
        void update(const unsigned long t) override;

        void setTemp(float temp);
        void setHumidity(float hum);

        float getTemp() const;
        float getHumidity() const;
    
        CollectedData* getTempData();
        CollectedData* getHumidityData();

    private:
        CollectedData _tempData;
        CollectedData _humidityData;
};

#endif