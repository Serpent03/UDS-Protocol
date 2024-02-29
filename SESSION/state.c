#include "../common.h"
#include "state.h"

uInt8 DEVICE_STATE[2];

void set_state(enum STATE_CODES state, uInt8 code) {
  DEVICE_STATE[state] = code;  
}

uInt8 get_state(enum STATE_CODES state) {
  return DEVICE_STATE[state];
}
