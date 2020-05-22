#include "DataModule.h"

DataModule::DataModule() : _tempData(), _humidityData() {
}

void DataModule::setup() {
}

void DataModule::update(const unsigned long t) {
    _tempData.update(t);
    _humidityData.update(t);
}

void DataModule::setTemp(float temp) {
    _tempData.setValue(temp);
}

float DataModule::getTemp() const {
    return _tempData.getValue();
}

void DataModule::setHumidity(float hum) {
    _humidityData.setValue(hum);
}

float DataModule::getHumidity() const {
    return _humidityData.getValue();
}

CollectedData* DataModule::getTempData() {
    return &_tempData;
}

CollectedData* DataModule::getHumidityData() {
    return &_humidityData;
}