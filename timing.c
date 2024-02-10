#include "common.h"
#include "timing.h"

/* This can stay hidden and work inside the functions. */
struct timeval tp;

uInt64 getTime() {
  gettimeofday(&tp, NULL);
  uInt64 ret_val = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
  return ret_val;
}

void setTime(uInt64 *CLOCK_VAR) {
  gettimeofday(&tp, NULL);
  *CLOCK_VAR = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}
