#include "DisplayModule.h"

DisplayModule::DisplayModule()
    : _display(DISPLAY_ADDR, SDA, SCL, GEOMETRY_128_32),
    _isOn(true) {
}

void DisplayModule::setup() {
    _display.init();
    _display.flipScreenVertically();
    _isOn = true;
}

void DisplayModule::setCurrentFrame(Frame* frame) {
    _currentFrame = frame;
}

void DisplayModule::update(unsigned long t) {
  if (_isOn && t >= nextDisplayUpdate) {
    _display.clear();
    if (_currentFrame) {
        _currentFrame->draw(&_display);
    } else {
        drawNoFrame();
    }

    _display.display();
    nextDisplayUpdate = t + DISPLAY_UPDATE_DELAY;
  }
}

void DisplayModule::displayOn() {
    if (!_isOn) {
        _display.displayOn();
        _isOn = true;
    }
}

void DisplayModule::displayOff() {
    if (_isOn) {
        _display.displayOff();
        _isOn = false;
    }
}

bool DisplayModule::isOn() {
    return _isOn;
}

void DisplayModule::drawNoFrame() {
    _display.setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_LEFT);
    _display.setFont(ArialMT_Plain_10);
    _display.drawString(0, 0, "No frame selected");
}
