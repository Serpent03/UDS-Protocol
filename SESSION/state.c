#include "../common.h"
#include "state.h"
#include "timing.h"
#include <string.h>

uInt8 DEVICE_STATE[3];
uInt64 LAST_CLIENT_CALL;
uInt64 LAST_SECURITY_CALL;

/** @todo set up ENUM codes for state conditions */

void set_last_client_call() {
  LAST_CLIENT_CALL = getTime();
}

void set_last_security_call() {
  LAST_SECURITY_CALL = getTime();
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
  if (DEVICE_STATE[STATE_DIAGNOSTIC_SESSION] == 0x3 || DEVICE_STATE[STATE_DIAGNOSTIC_SESSION] == 0x2) {
    // printf("LCC\n");
    /* Set device state back to default session. */
    if (!check_if_timeout(LAST_CLIENT_CALL, EXTENDED_SESSION_TIMEOUT)) { DEVICE_STATE[STATE_DIAGNOSTIC_SESSION] = 0x1; printf("PRIVILEGE TIMEOUT\n"); } 
  }
  if (DEVICE_STATE[STATE_SECURITY_SERVICE] == 0xFF) {
    /* Allow security requests again. */
    if (!check_if_timeout(LAST_SECURITY_CALL, SECURITY_REQUEST_TIMEOUT)) { DEVICE_STATE[STATE_SECURITY_SERVICE] = 0x00 ; printf("AVAILABLE FOR SEC TRY AGAIN\n"); }
  }
}

void state_initialize() {
  memset(DEVICE_STATE, 0, 3);
  set_state(STATE_DIAGNOSTIC_SESSION, 0x1); /* by default the program starts in the default diagnostic session */
  set_state(STATE_SECURITY_SERVICE, 0x0);
}
