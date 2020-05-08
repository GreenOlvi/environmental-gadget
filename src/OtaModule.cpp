#include "OtaModule.h"

OtaModule::OtaModule(const char* password, DisplayModule* display)
    : _password(password), _display(display) {
}

void OtaModule::setup() {
    ArduinoOTA.setPassword(_password);

    ArduinoOTA.onStart([this]() {
        _prevFrame = _display->getCurrentFrame();
        _display->displayOn();
        _display->setCurrentFrame(&_frame);
        _frame.text = "starting update...";
        _display->update(millis());
    });

    ArduinoOTA.onEnd([this]() {
        _frame.text = "finished";
        _display->update(millis());
    });

    ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
        _frame.progress = progress/(total + 0.0);
        _frame.text = "updating...";
        _display->update(millis());
    });

    ArduinoOTA.onError([this](ota_error_t error) {
        if (error == OTA_AUTH_ERROR) {
            _frame.text = "error: auth failed";
        } else if (error == OTA_BEGIN_ERROR) {
            _frame.text = "error: begin failed";
        } else if (error == OTA_CONNECT_ERROR) {
            _frame.text = "error: connect failed";
        } else if (error == OTA_RECEIVE_ERROR) {
            _frame.text = "error: receive failed";
        } else if (error == OTA_END_ERROR) {
            _frame.text = "error: end failed";
        }
        _display->update(millis());
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

void OtaFrame::draw(OLEDDisplay* display) {
    char perc[5];
    sprintf(perc, "%.0f%%", progress * 100);

    display->drawRect(0, 16, 128, 10);
    display->fillRect(0, 16, 128 * progress, 10);
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->drawString(0, 0, text);
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(128, 0, perc);
}