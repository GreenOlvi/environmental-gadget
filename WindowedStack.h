#ifndef WindowedStack_h
#define WindowedStack_h

#include "Arduino.h"

class WindowedStack
{
    public:
        WindowedStack(int size);
        void push(float value);
        float* getData();
        int Count();
        int WindowSize();
        // float GetValue(int i);
        ~WindowedStack();
    private:
        int _n, _i, _count;
        float* a;
};

#endif