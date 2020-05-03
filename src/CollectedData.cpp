#include "CollectedData.h"

CollectedData::CollectedData() : _value(0.), _minuteData(60), _hourData(60) {
}

RollingBuffer* CollectedData::getMinuteData() {
    return &_minuteData;
}

RollingBuffer* CollectedData::getHourData() {
    return &_hourData;
}

void CollectedData::update(const unsigned long t) {
    secondUpdate(t);
    minuteUpdate(t);
}

void CollectedData::setValue(float value) {
    _value = value;
}

float CollectedData::getValue() const {
    return _value;
}

void CollectedData::secondUpdate(const unsigned long t) {
    if (t - _lastSecondUpdate >= 1000) {
        _minuteData.push(_value);
        _lastSecondUpdate = t;
    }
}

void CollectedData::minuteUpdate(const unsigned long t) {
    if (t - _lastMinuteUpdate >= 60 * 1000) {
        _hourData.push(_minuteData.average());
        _lastMinuteUpdate = t;
    }
}