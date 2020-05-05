#include "SensorsModule.h"

SensorsModule::SensorsModule(DataModule* dataModule)
    : _data(dataModule), _dht(), _ds(OTHER_ONE_WIRE_BUS), _auxSensors(&_ds) {
}

void SensorsModule::setup() {
    _dht.setup(ONE_WIRE_BUS, DHTesp::AM2302);
    _auxSensors.begin();

    _dhtSamplingPeriod = _dht.getMinimumSamplingPeriod();
}

void SensorsModule::update(const unsigned long t) {
    if (t - _lastDhtUpdate >= _dhtSamplingPeriod) {
        TempAndHumidity th = _dht.getTempAndHumidity();
        _data->setTemp(th.temperature);
        _data->setHumidity(th.humidity);
        _lastDhtUpdate = t;
    }

    if (t - _lastAuxUpdate >= AUX_SENSOR_UPDATE_DELAY) {
        _auxSensors.requestTemperaturesByIndex(0);
        float tempC = _auxSensors.getTempCByIndex(0);
        float auxTemp = NAN;
        if (tempC != DEVICE_DISCONNECTED_C) {
            auxTemp = tempC;
        }
        _data->setAuxTemp(auxTemp);
        _lastAuxUpdate = t;
    }
}