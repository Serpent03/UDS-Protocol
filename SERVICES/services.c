#include "./services.h"

uInt8 resp_data[4096];

UDS_Packet* service_handler(UDS_Packet *rx, bool *silenceTx) {
  UDS_Packet *tx; 
  uInt8 response_code = 0x00;
  uInt16 idx = 0;

  switch (rx->SID) {
    case SID_DIAGNOSTIC_SESS_CNTL:
      if (!handle_diag_sess_cntl(rx, resp_data, &idx)) {
        response_code = NRC_NEGATIVE_RESPONSE;
      } else {
        response_code = rx->SID + 0x40;
      }
      break;
    default:
      response_code = 0x00;
      break;
  }
  tx = generate_UDS_packet(response_code, resp_data, idx);
  return tx;
}
