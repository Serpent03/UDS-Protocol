#include "../common.h"
#include "services.h"
#include "../SESSION/session.h"

void handle_debug(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  resp_data[*(idx)++] = isTransmitter; // scope for expansion in the future
} 
