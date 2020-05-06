#ifndef InputModule_h
#define InputModule_h

#include "Arduino.h"
#include "Module.h"

typedef void (*inputHandler)(const unsigned long);

class InputModule : public Module {
    public:
        InputModule(const int buttonPin);
        void setup(void) override;
        void update(const unsigned long t) override;
        void onButtonPress(const inputHandler);
        void onButtonRelease(const inputHandler);

        bool buttonIsPressed(void);
        unsigned long lastButtonChange(void);
    private:
        unsigned int _debounceDelay = 50;
        const int _buttonPin;
        inputHandler _onPress = NULL;
        inputHandler _onRelease = NULL;

        int _buttonState;
        int _lastButtonState;
        int _lastDebounceTime;

        unsigned long _lastButtonChange;
};

#endif