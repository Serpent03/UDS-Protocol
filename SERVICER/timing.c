#include "../common.h"
#include "timing.h"
#include <unistd.h>

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

bool check_if_timeout(uInt64 CLOCK_TIME, enum ISO_TP_TIME_LIMITS time_limit) {
  return getTime() < CLOCK_TIME + time_limit;
}

void _sleep(uInt64 ms) {
  uInt64 curtime;
  setTime(&curtime);
  while (getTime() < curtime + ms) {
    ;
  }
}
