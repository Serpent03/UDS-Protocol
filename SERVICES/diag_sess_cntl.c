#include "services.h"
#include "../UDS/UDS.h"

void handle_diag_sess_cntl(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  // Pointers are tricky..
  resp_data[(*idx)++] = rx->SID;
  for (uInt8 i = 0; i < 4; i++) {
    resp_data[(*idx)++] = rx->data[i]; // mock SFB and other data 
  }
  /** @todo Branch out the diagnostic session into separate SFBs etc. */
}
