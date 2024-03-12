#include "services.h"
#include "../SESSION/state.h"
#include "../SESSION/timing.h"
#include "../UDS/UDS.h"
#include "../UTILS/utils.h"

enum {
  RQST_LV_1 = 0x1,
  RQST_LV_3 = 0x3,
  RQST_LV_5 = 0x5
};

/**
 * @todo implement security level request
 * @todo implement seed generation
 * @todo implement seed validation
 * @todo implement negative seed response timeout
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

bool handle_access_escalation(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  uInt8 current_security_level = get_state(STATE_SECURITY_SERVICE);
  uInt8 request_security_level = rx->data[0];
  bool is_requesting_seed = rx->data[0] % 2 != 0; /* Only odd SFBs request seed */

  if (current_security_level == 0xFF) {
    /* Currently in timeout delay */
    set_failure(rx, resp_data, idx, NRC_TIME_DELAY_NOT_EXPIRED);
    return false;
  }
  if (is_requesting_seed) {
    if (request_security_level == current_security_level + 1) {
      /* Return the seed. */
      insertIntoArray(resp_data, SECURITY_SEED >> 24 & 0xFF, idx);
      insertIntoArray(resp_data, SECURITY_SEED >> 16 & 0xFF, idx);
      insertIntoArray(resp_data, SECURITY_SEED >> 8 & 0xFF, idx);
      insertIntoArray(resp_data, SECURITY_SEED & 0xFF, idx);
      return true;
    } else if (request_security_level < current_security_level) {
      /** @todo return an error here..? We've already accessed this level. Do we ignore this request? */
      return true;
    } else {
      set_failure(rx, resp_data, idx, NRC_REQUEST_SEQUENCE_ERROR);
    }
  } else {
    printf("REQ\n");
    /* Tester is trying to validate his key against our secret key now. */
    uInt32 client_request_key = rx->data[1] << 24 | rx->data[2] << 16 | rx->data[3] << 8 | rx->data[4];
    if (client_request_key == SECURITY_KEY) {
      set_state(STATE_SECURITY_SERVICE, current_security_level + 2);
      return true;
    } else if (request_security_level < current_security_level) {
      /* We are already at that security level */
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
  if (rx->data[0] % 2 == 0 && rx->dataLength != 5) {
    set_failure(rx, resp_data, idx, NRC_SUB_FUNCTION_NOT_SUPPLIED);
    return false;
  }
  insertIntoArray(resp_data, rx->data[0], idx);
  if (!handle_access_escalation(rx, resp_data, idx)) {
    uInt8 current_security_level = get_state(STATE_SECURITY_SERVICE);
    if (current_security_level + 0x10 >= 0x30) {
      set_state(STATE_SECURITY_SERVICE, 0xFF); /* if we've exhausted more than three tries. */
    } else {
      printf("TIMEOUT ON SECURITY SERVICE!\n"); /** @debug */
      set_state(STATE_SECURITY_SERVICE, current_security_level + 0x10); /* we increment 0x01 to 0x11 on a failure. */
    }
    return false;
  }
  return true;
}
