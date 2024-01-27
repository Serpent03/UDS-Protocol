#include "common.h"

UDS_Packet* generate_UDS_packet(unsigned char SID, unsigned char *data, unsigned char dataLength) {
  UDS_Packet  *udsp = (UDS_Packet*)calloc(1, sizeof(UDS_Packet));
  udsp->SID = SID;
  udsp->data = data;
  udsp->dataLength = dataLength;
  return udsp;
}

UDS_Packet* parse(UDS_Packet *udsp) {
  unsigned char response_code = 0x00;
  unsigned char *data = (unsigned char*)calloc(4, sizeof(unsigned char*));
  unsigned char idx = 0;
  // we will realloc() this thing for now, as data comes by. although this approach feels janky

  // "standard" response; mock reply
  switch (udsp->SID) {
    case SID_DIAGNOSTIC_SESS_CNTL:
      if (udsp->dataLength == 0) {
        response_code = NRC_NEGATIVE_RESPONSE;
        data[idx++] = NRC_INCORRECT_MESSAGE_LENGTH;
      }
      break;
    default:
      response_code = udsp->SID + 0x40;
      break;
  }
  if (idx == 0) {
    // we can just pass in a NULL ptr at this stage and nothing will go wrong - there hasn't been any
    // data generated after the ECU has replied, so it may be safe to assume that the ECU will not reply 
    // at all with additional data.
    free(data);
  } else {
    data = realloc(data, (size_t)idx);
  }

  UDS_Packet* response = generate_UDS_packet(response_code, data, idx);
  return response;
}
