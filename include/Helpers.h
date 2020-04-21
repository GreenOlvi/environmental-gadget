#ifndef Helpers_h
#define Helpers_h

#include "WindowedStack.h"

#define _countof(array) (sizeof(array) / sizeof(array[0]))

float average(float* data, int dataLength);
float average(WindowedStack* data);

#endif