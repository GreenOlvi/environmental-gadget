#include "RollingBuffer.h"

RollingBuffer::RollingBuffer(int size) : _n(size), _i(0), _count(0) {
    _buffer = (float *)malloc(sizeof(float) * size * 2);
}

void RollingBuffer::push(float value) {
    _buffer[_i] = value;
    _buffer[_i + _n] = value;

    _i++;
    if (_i >= _n)
    {
        _i = 0;
    }

    if (_count < _n) {
        _count++;
    }
}

float* RollingBuffer::getData() const {
    return _count < _n ? _buffer : _buffer + _i;
}

int RollingBuffer::count() const {
    return _count;
}

float RollingBuffer::average() const {
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

RollingBuffer::~RollingBuffer() {
    free(_buffer);
}