#include "services.h"
#include "../SESSION/state.h"
#include "../SESSION/timing.h"
#include "../UDS/UDS.h"
#include "../UTILS/utils.h"

enum SFB_CODES {
  /* Each SID file has its own SFB codes. */
  DEFAULT_SESSION = 0x1,
  PROGRAM_SESSION = 0x2,
  EXTENDED_SESSION = 0x3,
  SAFETY_SESSION = 0x4,
};

void programming_session(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  set_state(STATE_DIAGNOSTIC_SESSION, PROGRAM_SESSION);
}

void extended_session(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  set_state(STATE_DIAGNOSTIC_SESSION, EXTENDED_SESSION);
}

void default_session(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  set_state(STATE_DIAGNOSTIC_SESSION, DEFAULT_SESSION);

  insertIntoArray(resp_data, (ISOTP_P2 >> 8) & 0xFF, idx);
  insertIntoArray(resp_data, (ISOTP_P2) & 0xFF, idx);
  insertIntoArray(resp_data, (ISOTP_P2_EXT >> 8), idx);
  insertIntoArray(resp_data, (ISOTP_P2_EXT) & 0xFF, idx);
  /** @todo generate P2 and P2extended values */
  /** @todo verify consistency in the P2 reply data format */ 
}

void safety_session(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  set_state(STATE_DIAGNOSTIC_SESSION, SAFETY_SESSION);
}

bool handle_diag_sess_cntl(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  // Pointers are tricky..
  if (rx->dataLength == 0) {
    set_failure(rx, resp_data, idx, NRC_INCORRECT_MESSAGE_LENGTH);
    return false;
  }
  insertIntoArray(resp_data, rx->data[0], idx);
  switch (rx->data[0]) {
    case PROGRAM_SESSION:
      programming_session(rx, resp_data, idx);
      break;
    case DEFAULT_SESSION:
      default_session(rx, resp_data, idx);
      break;
    case EXTENDED_SESSION:
      extended_session(rx, resp_data, idx);
      break;
    case SAFETY_SESSION:
      safety_session(rx, resp_data, idx);
      break;
    default:
      set_failure(rx, resp_data, idx, NRC_SUB_FUNCTION_NOT_SUPPLIED);
      return false;
  }
  return true;
}
