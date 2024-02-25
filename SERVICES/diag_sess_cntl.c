#include "services.h"
#include "../UDS/UDS.h"

bool handle_diag_sess_cntl(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  // Pointers are tricky..
  resp_data[(*idx)++] = rx->SID;
  if (rx->dataLength == 0) {
    resp_data[(*idx)++] = NRC_INCORRECT_MESSAGE_LENGTH; 
    return false;
  }
  for (uInt8 i = 0; i < 4; i++) {
    resp_data[(*idx)++] = rx->data[i]; // mock SFB and other data 
  }
  /** @todo Branch out the diagnostic session into separate SFBs etc. */
  return true;
}
