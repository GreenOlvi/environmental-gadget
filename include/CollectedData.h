#ifndef CollectedData_h
#define CollectedData_h

#include "Updatable.h"
#include "RollingBuffer.h"

class CollectedData : public Updatable {
    public:
        CollectedData();

        RollingBuffer* getMinuteData();
        RollingBuffer* getHourData();

        void update(const unsigned long t) override;
        void setValue(float value);
        float getValue() const;

    private:
        float _value;

        RollingBuffer _minuteData;
        RollingBuffer _hourData;

        unsigned long _lastSecondUpdate = 0;
        void secondUpdate(const unsigned long t);

        unsigned long _lastMinuteUpdate = 0;
        void minuteUpdate(const unsigned long t);
};

#endif