#include "services.h"
#include "../SESSION/state.h"
#include "../UDS/UDS.h"

enum SFB_CODES {
  /* Each SID file has its own SFB codes. */
  HARD_RESET = 0x1,
  KEY_OFF_ON = 0x2,
  SOFT_RESET = 0x3,
  ENABLE_RAPID_POWER_SHUTDOWN = 0x4,
  DISABLE_RAPID_POWER_SHUTDOWN = 0x5,
};

/* STUB */
void hard_reset() {}
void key_off_on_reset() {}
void soft_reset() {}
void enable_rapid_power_shutdown() {}
void disable_rapid_power_shutdown() {}

bool handle_ecu_reset(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  if (rx->dataLength == 0) {
    set_failure(rx, resp_data, idx, NRC_INCORRECT_MESSAGE_LENGTH);
    return false;
  }
  if (rx->data[0] > 0x05) {
    set_failure(rx, resp_data, idx, NRC_REQUEST_OUT_OF_RANGE);
    return false;
  }
  resp_data[(*idx)++] = rx->data[0];
  return true;
}
