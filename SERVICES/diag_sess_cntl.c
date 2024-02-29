#include "services.h"
#include "../SESSION/state.h"
#include "../UDS/UDS.h"

void programming_session(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  set_state(STATE_DIAG_SESSION, 0x2);
}

void extended_session(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  set_state(STATE_DIAG_SESSION, 0x3);
}

void default_session(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  set_state(STATE_DIAG_SESSION, 0x1);
}

void safety_session(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  set_state(STATE_DIAG_SESSION, 0x4);
}

bool handle_diag_sess_cntl(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  // Pointers are tricky..
  if (rx->dataLength == 0) {
    set_failure(rx, resp_data, idx, NRC_INCORRECT_MESSAGE_LENGTH);
    return false;
  }
  if (rx->data[0] >= 0x05) {
    set_failure(rx, resp_data, idx, NRC_REQUEST_OUT_OF_RANGE);
    return false;
  }
  for (uInt8 i = 0; i < 1; i++) {
    resp_data[(*idx)++] = rx->data[i]; // mock SFB and other data 
  }
  switch (rx->data[0]) {
    case SID10_PROGRAM_SESSION:
      programming_session(rx, resp_data, idx);
      break;
    case SID10_DEFAULT_SESSION:
      default_session(rx, resp_data, idx);
      break;
    case SID10_EXTENDED_SESSION:
      extended_session(rx, resp_data, idx);
      break;
    case SID10_SAFETY_SESSION:
      safety_session(rx, resp_data, idx);
      break;
  }
  return true;
}
