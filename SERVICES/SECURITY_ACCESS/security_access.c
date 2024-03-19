#include "../services.h"
#include "../../SESSION/state.h"
#include "../../SESSION/timing.h"
#include "../../UDS/UDS.h"
#include "../../UTILS/utils.h"
#include "security_access.h"
#include <string.h>

#define SEED_SIZE 8

enum {
  RQST_LV_1 = 0x1,
  RQST_LV_3 = 0x3,
  RQST_LV_5 = 0x5,
  RQST_LV_7 = 0x7
};

typedef struct SECURITY_SETTINGS {
  uInt8 SEED[SEED_SIZE];
  uInt8 KEY[SEED_SIZE];
} SECURITY_SETTINGS;

SECURITY_SETTINGS security_table[4];
uInt8 SEED_1[SEED_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8};
uInt8 SEED_3[SEED_SIZE] = {11, 12, 13, 14, 15, 16, 17, 18};
uInt8 SEED_5[SEED_SIZE] = {21, 22, 23, 24, 25, 26, 27, 28};
uInt8 SEED_7[SEED_SIZE] = {31, 32, 33, 34, 35, 36, 37, 38};

uInt8 KEY_1[SEED_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8};
uInt8 KEY_3[SEED_SIZE] = {11, 12, 13, 14, 15, 16, 17, 18};
uInt8 KEY_5[SEED_SIZE] = {21, 22, 23, 24, 25, 26, 27, 28};
uInt8 KEY_7[SEED_SIZE] = {31, 32, 33, 34, 35, 36, 37, 38};

uInt8 SEED[SEED_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8};
uInt8 KEY[SEED_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8};

void initialize_security_tables() {
  memcpy(&security_table[0].SEED[0], SEED_1, SEED_SIZE);
  memcpy(&security_table[1].SEED[0], SEED_3, SEED_SIZE);
  memcpy(&security_table[2].SEED[0], SEED_3, SEED_SIZE);
  memcpy(&security_table[3].SEED[0], SEED_3, SEED_SIZE);

  memcpy(&security_table[0].KEY[0], KEY_1, SEED_SIZE);
  memcpy(&security_table[1].KEY[0], KEY_3, SEED_SIZE);
  memcpy(&security_table[2].KEY[0], KEY_3, SEED_SIZE);
  memcpy(&security_table[3].KEY[0], KEY_3, SEED_SIZE);
}

/**
 * @todo implement seed generation function
 * @todo implement seed validation function
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

void seed_generation(uInt8 *resp_data, uInt16 *idx, uInt8 request_level) {
  /* STUB */
  uInt8 n = (request_level - 1) / 2;
  for (uInt8 i = 0; i < 8; i++) {
    insertIntoArray(resp_data, security_table[n].SEED[i], idx);
  }
}

bool seed_validation(uInt8 *client_key, uInt8 request_level) {
  /* STUB */
  uInt8 n = (request_level - 1) / 2; /* Reduce odd numbers to a linear map: 0=1, 1=3, 2=5, 3=7 */
  for (uInt8 i = 0; i < 8; i++) {
    printf("%d == %d\n", client_key[i], security_table[n].KEY[i]);
  }
  return memcmp(client_key, &security_table[n].KEY, sizeof(SEED)) == 0;
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
  printf("CURRENT SEC LEVEL: %02xh\n", current_security_level);
  printf("REQUEST SEC LEVEL: %02xh\n", request_security_level);

  if (current_security_level == 0xFF) {
    /* Currently in timeout delay */
    set_failure(rx, resp_data, idx, NRC_TIME_DELAY_NOT_EXPIRED);
    return false;
  }
  if (is_requesting_seed) {
    if (request_security_level == current_security_level + 1) {
      /* Return the seed. */
      seed_generation(resp_data, idx, request_security_level);
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
    if (seed_validation(client_key, request_security_level) && request_security_level == current_security_level + 1) {
      set_state(STATE_SECURITY_SERVICE, current_security_level + 1);
      return true;
    } else {
      set_failure(rx, resp_data, idx, NRC_INVALID_KEY);
      return false;
    }
  }
  return false; /* All successful operations must be returned inside the if/else scope. */ 
}

bool security_check_security_access(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx, uInt8 dgSess, uInt8 secAcess) {
  if (get_state(STATE_DIAGNOSTIC_SESSION) < 0x2) {
    set_failure(rx, resp_data, idx, NRC_SERVICE_NOT_SUPPORTED);
    return false;
  }
  return true;
}

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
  if (rx->data[0] % 2 != 0) {
    switch(rx->data[0]) {
      case RQST_LV_1:
        break;
      case RQST_LV_3:
        break;
      case RQST_LV_5:
        break;
      case RQST_LV_7:
        break;
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
