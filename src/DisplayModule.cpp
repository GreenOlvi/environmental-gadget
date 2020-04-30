#include "DisplayModule.h"

DisplayModule::DisplayModule()
    : _display(DISPLAY_ADDR, SDA, SCL, GEOMETRY_128_32) {
}

void DisplayModule::setup() {
    _display.init();
    _display.flipScreenVertically();
}

OLEDDisplay* DisplayModule::getDisplay() {
    return &_display;
}

void DisplayModule::setCurrentFrame(Frame* frame) {
    _currentFrame = frame;
}

void DisplayModule::update(unsigned long t) {
  if (t >= nextDisplayUpdate) {
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

void DisplayModule::drawNoFrame() {
    _display.setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_LEFT);
    _display.setFont(ArialMT_Plain_10);
    _display.drawString(0, 0, "No frame selected");
}
