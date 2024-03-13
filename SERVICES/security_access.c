#include "services.h"
#include "../SESSION/state.h"
#include "../SESSION/timing.h"
#include "../UDS/UDS.h"
#include "../UTILS/utils.h"

enum {
  RQST_LV_1 = 0x1,
  RQST_LV_3 = 0x3,
  RQST_LV_5 = 0x5,
  RQST_LV_7 = 0x7
};

uInt8 SEED[8] = {1, 2, 3, 4, 5, 6, 7, 8};
uInt8 KEY[8] = {1, 2, 3, 4, 5, 6, 7, 8};

/**
 * @todo implement seed generation function
 * @todo implement seed validation function
 * @todo implement negative seed response timeout
 * @todo extended diagnostic session/programming session before security access.
 */

/**
 * @brief Security access state codes
 * 0x01: requesting permission for first level
 * 0x02: granted permission for first level
 * 0x03: requesting permission for second level
 * 0x04: granted permission for second level
 * ..
 * ..
 * ..
 * 0xFF: request timeout
 * First 4 bits set the current try number.
 * 0x01 means first try for first level.
 * 0x11 means second try for first level.
 * 0x21 means third try for first level.
 * The state enters request timeout when exceeding 3 tries.
 */

void seed_generation(uInt8 *resp_data, uInt16 *idx) {
  /* STUB */
  for (uInt8 i = 0; i < 8; i++) {
    insertIntoArray(resp_data, SEED[i], idx);
  }
}

bool seed_validation(uInt8 *client_key) {
  /* STUB */
  for (uInt8 i = 0; i < 8; i++) {
    printf("%d == %d\n", client_key[i], KEY[i]);
  }
  return true;
}

bool handle_failure(bool result) {
  if (result) return true; /* Return true if the operation was successful. */

  uInt8 current_security_level = get_state(STATE_SECURITY_SERVICE);
  if (current_security_level + 0x10 >= 0x30) {
    set_state(STATE_SECURITY_SERVICE, 0xFF); /* if we've exhausted more than three tries. */
  } else {
    printf("ERROR + 1\n"); /** @debug */
    set_state(STATE_SECURITY_SERVICE, current_security_level + 0x10); /* we increment 0x01 to 0x11 on a failure. */
  }

  return false;
}

bool handle_access_escalation(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  uInt8 current_security_level = get_state(STATE_SECURITY_SERVICE);
  uInt8 request_security_level = rx->data[0];
  bool is_requesting_seed = rx->data[0] % 2 != 0; /* Only odd SFBs request seed */

  /* Set the security call, for accurate time exhaustion limits. */
  set_last_security_call();

  if (current_security_level == 0xFF) {
    /* Currently in timeout delay */
    set_failure(rx, resp_data, idx, NRC_TIME_DELAY_NOT_EXPIRED);
    return false;
  }
  if (is_requesting_seed) {
    if (request_security_level == current_security_level + 1) {
      /* Return the seed. */
      seed_generation(resp_data, idx);
      set_state(STATE_SECURITY_SERVICE, current_security_level + 1);
      return true;
    } else if (request_security_level < current_security_level) {
      set_failure(rx, resp_data, idx, NRC_SERVICE_NOT_SUPPORTED);
      return false;
    } else {
      set_failure(rx, resp_data, idx, NRC_REQUEST_SEQUENCE_ERROR);
    }
  } else {
    /* Tester is trying to validate his key against our secret key now. */
    uInt8 *client_key = &rx->data[1]; /* point to the data starting after the SFB. */
    if (seed_validation(client_key) && request_security_level == current_security_level + 1) {
      set_state(STATE_SECURITY_SERVICE, current_security_level + 1);
      return true;
    } else {
      set_failure(rx, resp_data, idx, NRC_INVALID_KEY);
      return false;
    }
  }
  return false; /* All successful operations must be returned inside the if/else scope. */ 
}

/** @todo fix negative response mechanism(security request timeout) */

bool handle_security_access(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  if (rx->dataLength < 1) {
    set_failure(rx, resp_data, idx, NRC_INCORRECT_MESSAGE_LENGTH);
    return false;
  }
  if (rx->data[0] % 2 == 0 && rx->dataLength < 2) {
    /* Assuming that the SFB + SEED/KEY will be atleast 2 bytes in length. */
    set_failure(rx, resp_data, idx, NRC_INCORRECT_MESSAGE_LENGTH);
    return false;
  }
  if (get_state(STATE_DIAGNOSTIC_SESSION) < 0x2) {
    set_failure(rx, resp_data, idx, NRC_SERVICE_NOT_SUPPORTED);
    return false;
  }
  if (rx->data[0] % 2 != 0) {
    switch(rx->data[0]) {
      default:
        set_failure(rx, resp_data, idx, NRC_SERVICE_NOT_SUPPORTED);
        return false;
    }
  }
  insertIntoArray(resp_data, rx->data[0], idx);
  /* Handle the error inside the escalation function. 
   * Returns true if the function did not error out. */
  return handle_failure(handle_access_escalation(rx, resp_data, idx));
}
