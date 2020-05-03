#ifndef RollingBuffer_h
#define RollingBuffer_h

#include <Arduino.h>

class RollingBuffer
{
    public:
        explicit RollingBuffer(const int size);
        void push(const float value);
        float* getData() const;
        int count() const;
        float average() const;
        ~RollingBuffer();
    private:
        const int _n;
        int _i, _count;
        float* _buffer;
};

#endif