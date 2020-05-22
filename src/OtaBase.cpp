#include "OtaBase.h"

OtaBase::OtaBase(const char* password)
    : _password(password) {
}

void OtaBase::setup() {
    ArduinoOTA.setPassword(_password);
}

void OtaBase::update(const unsigned long t) {
    if (t - _lastUpdate > OTA_UPDATE_DELAY) {
        if (WiFi.isConnected()) {
            if (!_isStarted) {
                ArduinoOTA.begin();
                _isStarted = true;
            } else {
                ArduinoOTA.handle();
            }
        }
        _lastUpdate = t;
    }
}