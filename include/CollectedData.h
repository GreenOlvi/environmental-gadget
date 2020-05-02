#ifndef CollectedData_h
#define CollectedData_h

#include "Updatable.h"
#include "WindowedStack.h"

class CollectedData : public Updatable {
    public:
        CollectedData();

        WindowedStack* getMinuteData();
        WindowedStack* getHourData();

        void update(unsigned long t) override;
        void setValue(float value);
        float getValue();
    private:
        float _value;

        WindowedStack _minuteData;
        WindowedStack _hourData;

        unsigned long _lastSecondUpdate = 0;
        void secondUpdate(unsigned long t);

        unsigned long _lastMinuteUpdate = 0;
        void minuteUpdate(unsigned long t);
};

#endif