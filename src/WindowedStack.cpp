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

int WindowedStack::count() {
    return _count;
}

float WindowedStack::average() {
    float* data = getData();
    float sum = 0.;
    int count = 0;
    for (int i = 0; i < _count; i++) {
        if (data[i] != NAN) {
        sum += data[i];
        count++;
        }
    }

    if (count > 0) {
        return sum / count;
    } else {
        return NAN;
    }
}

int WindowedStack::windowSize() {
    return _n;
}

WindowedStack::~WindowedStack() {
    free(a);
}