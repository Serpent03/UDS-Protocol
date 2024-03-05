#include "../common.h"
#include "state.h"
#include "timing.h"

uInt8 DEVICE_STATE[2];
uInt64 LAST_CLIENT_CALL;

void set_state(enum STATE_CODES state, uInt8 code) {
  DEVICE_STATE[state] = code;  
  /** @todo verify if it is last client call or last tester present call for extended diagnostic session. */
  LAST_CLIENT_CALL = getTime();
}

uInt8 get_state(enum STATE_CODES state) {
  return DEVICE_STATE[state];
}

void update_state() {
  if (DEVICE_STATE[0] == 0x3) {
    if (!check_if_timeout(LAST_CLIENT_CALL, EXTENDED_SESSION_TIMEOUT)) DEVICE_STATE[0] = 0x0; /* Set device state back to default session. */
  }
}
