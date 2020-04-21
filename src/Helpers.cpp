#include <math.h>
#include "WindowedStack.h"
#include "Helpers.h"

float average(float* data, int dataLength) {
  float sum = 0.;
  int count = 0;
  for (int i = 0; i < dataLength; i++) {
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

float average(WindowedStack* data) {
    return average(data->getData(), data->Count());
}