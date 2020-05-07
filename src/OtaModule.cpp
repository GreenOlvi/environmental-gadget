#include "OtaModule.h"

OtaModule::OtaModule(const char* password) : _password(password), _isStarted(false) {
}

void OtaModule::setup() {
    ArduinoOTA.setPassword(_password);

    ArduinoOTA.onStart([]() {
    });

    ArduinoOTA.onEnd([]() {
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    });

    ArduinoOTA.onError([](ota_error_t error) {
    });
}

void OtaModule::update(const unsigned long t) {
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