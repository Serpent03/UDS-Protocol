#include "./services.h"

uInt8 resp_data[4096];

UDS_Packet* service_handler(UDS_Packet *rx, bool *silenceTx) {
  UDS_Packet *tx; 
  uInt8 response_code = 0x00;
  uInt16 idx = 0;

  switch (rx->SID) {
    case SID_DIAGNOSTIC_SESS_CNTL:
      if (rx->dataLength == 0) {
        response_code = NRC_NEGATIVE_RESPONSE;
        resp_data[idx++] = NRC_INCORRECT_MESSAGE_LENGTH;
      } else {
        response_code = rx->SID + 0x40;
        resp_data[idx++] = rx->SID;
      }
      break;
    default:
      response_code = rx->SID + 0x40;
      break;
  }
  tx = generate_UDS_packet(response_code, resp_data, idx);
  return tx;
}
