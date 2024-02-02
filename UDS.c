#include "common.h"
#include "UDS.h"

/**
 * uds_tx will be used for all outbound transmissions.
 */

UDS_Packet uds_tx;
uInt8 resp_data[4096];

/**
 * @todo Re-initialize the statically allocated memory if needed.
 */

UDS_Packet* generate_UDS_packet(uInt8 SID, uInt8 *data, uInt16 dataLength) {
  uds_tx.SID = SID;
  // uds_tx.data = data;
  memcpy(uds_tx.data, data, dataLength);
  uds_tx.dataLength = dataLength;
  return &uds_tx;
}

UDS_Packet* parse(UDS_Packet *udsp) {
  uInt8 response_code = 0x00;
  uInt8 idx = 0;
  // we will realloc() this thing for now, as data comes by. although this approach feels janky

  // "standard" response; mock reply
  switch (udsp->SID) {
    case SID_DIAGNOSTIC_SESS_CNTL:
      if (udsp->dataLength == 0) {
        response_code = NRC_NEGATIVE_RESPONSE;
        resp_data[idx++] = NRC_INCORRECT_MESSAGE_LENGTH;
      }
      break;
    default:
      response_code = udsp->SID + 0x40;
      break;
  }
  UDS_Packet* response = generate_UDS_packet(response_code, resp_data, idx);
  return response;
}
