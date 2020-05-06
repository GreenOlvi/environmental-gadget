#include "InputModule.h"

InputModule::InputModule(const int buttonPin)
    : _buttonPin(buttonPin), _buttonState(HIGH), _lastButtonState(_buttonState) {
}

void InputModule::setup() {
    pinMode(_buttonPin, INPUT_PULLUP);
}

void InputModule::update(const unsigned long t) {
    int read = digitalRead(_buttonPin);

    if (read != _lastButtonState) {
        _lastDebounceTime = t;
        _lastButtonState = read;
    }

    if (t - _lastDebounceTime > _debounceDelay) {
        if (read != _buttonState) {
            _buttonState = read;
            _lastButtonChange = t;

            if (_buttonState == LOW) {
                if (_onPress) {
                    _onPress(t);
                }
            } else {
                if (_onRelease) {
                    _onRelease(t);
                }
            }
        }
    }
}

void InputModule::onButtonPress(const inputHandler handler) {
    _onPress = handler;
}

void InputModule::onButtonRelease(const inputHandler handler) {
    _onRelease = handler;
}

bool InputModule::buttonIsPressed() {
    return _buttonState == LOW;
}

unsigned long InputModule::lastButtonChange() {
    return _lastButtonChange;
}