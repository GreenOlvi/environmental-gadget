#include "Arduino.h"
#include "WindowedStack.h"

WindowedStack::WindowedStack(int size) {
    _n = size;
    _i = 0;
    _count = 0;
    a = (float *)malloc(sizeof(float) * size * 2);
}

void WindowedStack::push(float value) {
    a[_i] = value;
    a[_i + _n] = value;

    _i++;
    if (_i >= _n)
    {
        _i = 0;
    }

    if (_count < _n) {
        _count++;
    }
}

float* WindowedStack::getData() {
    return _count < _n ? a : a + _i;
}

int WindowedStack::Count() {
    return _count;
}

int WindowedStack::WindowSize() {
    return _n;
}

WindowedStack::~WindowedStack() {
    free(a);
}