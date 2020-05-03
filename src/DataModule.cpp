#include "DataModule.h"

DataModule::DataModule() : _tempData(), _auxTempData(), _humidityData() {
}

void DataModule::setup() {
}

void DataModule::update(unsigned long t) {
    _tempData.update(t);
    _auxTempData.update(t);
    _humidityData.update(t);
}

void DataModule::setTemp(float temp) {
    _tempData.setValue(temp);
}

float DataModule::getTemp() const {
    return _tempData.getValue();
}

void DataModule::setAuxTemp(float temp) {
    _auxTempData.setValue(temp);
}

float DataModule::getAuxTemp() const {
    return _auxTempData.getValue();
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

CollectedData* DataModule::getAuxTempData() {
    return &_auxTempData;
}

CollectedData* DataModule::getHumidityData() {
    return &_humidityData;
}