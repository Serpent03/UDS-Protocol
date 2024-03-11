#include "../common.h"
#include "state.h"
#include "timing.h"

uInt8 DEVICE_STATE[2];
uInt64 LAST_CLIENT_CALL;

/** @todo set up ENUM codes for state conditions */

void set_last_client_call() {
  LAST_CLIENT_CALL = getTime();
}

void set_state(enum STATE_CODES state, uInt8 code) {
  DEVICE_STATE[state] = code;  
  set_last_client_call();
  /** @todo verify if it is last client call or last tester present call for extended diagnostic session. */
}

uInt8 get_state(enum STATE_CODES state) {
  return DEVICE_STATE[state];
}

void update_state() {
  if (DEVICE_STATE[0] == 0x3 || DEVICE_STATE[0] == 0x2) {
    // printf("LCC\n");
    if (!check_if_timeout(LAST_CLIENT_CALL, EXTENDED_SESSION_TIMEOUT)) { DEVICE_STATE[0] = 0x1; printf("PRIVILEGE TIMEOUT\n"); } /* Set device state back to default session. */
  }
}
