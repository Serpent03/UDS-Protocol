#include "./services.h"
#include "../SESSION/state.h"

uInt8 resp_data[4096];

void set_failure(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx, enum NEG_RESPONSE_CODES fail_code) {
  // limit the length to two automatically for sending failure data.
  resp_data[0] = rx->SID;
  resp_data[1] = fail_code;
  *idx = 2;
}

UDS_Packet* service_handler(UDS_Packet *rx, bool *silenceTx) {
  UDS_Packet *tx; 
  uInt8 response_code = 0x00;
  uInt16 idx = 0;

  /* We call this to establish the fact that the last communication happened. */
  /* because this function can only be accessed in the case of a valid communication. */
  set_last_client_call();

  switch (rx->SID) {
    case SID_STATE_DEBUG:
      handle_debug(rx, resp_data, &idx);
      response_code = 0xFF;
      break;
    case SID_DIAGNOSTIC_SESS_CNTL:
      if (!handle_diag_sess_cntl(rx, resp_data, &idx)) {
        response_code = NRC_NEGATIVE_RESPONSE;
      } else {
        response_code = rx->SID + 0x40;
      }
      break;
    case SID_ECU_RESET:
      if (!handle_ecu_reset(rx, resp_data, &idx)) {
        response_code = NRC_NEGATIVE_RESPONSE;
      } else {
        response_code = rx->SID + 0x40;
      }
      break;
    case SID_TESTER_PRESENT:
      if (!handle_tester_present(rx, resp_data, &idx)) {
        response_code = NRC_NEGATIVE_RESPONSE;
      } else {
        response_code = rx->SID + 0x40;
      }
      break;
    case SID_SECURITY_ACCESS_SERVICE:
      if (!handle_security_access(rx, resp_data, &idx)) {
        response_code = NRC_NEGATIVE_RESPONSE;
      } else {
        response_code = rx->SID + 0x40;
      }
      break;
    default:
      response_code = 0x00;
      idx = 0;
      break;
  }
  tx = generate_UDS_packet(response_code, resp_data, idx);
  return tx;
}
