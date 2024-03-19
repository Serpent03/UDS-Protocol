#include "../services.h"
#include "../../SESSION/state.h"
#include "../../UDS/UDS.h"
#include "../../UTILS/utils.h"
#include "ecu_reset.h"

enum SFB_CODES {
  /* Each SID file has its own SFB codes. */
  HARD_RESET = 0x1,
  KEY_OFF_ON = 0x2,
  SOFT_RESET = 0x3,
  ENABLE_RAPID_POWER_SHUTDOWN = 0x4,
  DISABLE_RAPID_POWER_SHUTDOWN = 0x5,
};

void hard_reset(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  /* Reset the micro-controller? This sounds like a job for the HAL layer, perhaps */
}
void key_off_on_reset(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  /* STUB */
}

void soft_reset(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  Server_Init();
}

void enable_rapid_power_shutdown(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  /* STUB */
}

void disable_rapid_power_shutdown(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  /* STUB */
}

bool handle_ecu_reset(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  // 1 byte
  if (rx->dataLength == 0) {
    set_failure(rx, resp_data, idx, NRC_INCORRECT_MESSAGE_LENGTH);
    return false;
  }
  insertIntoArray(resp_data, rx->data[0], idx);
  switch (rx->data[0]) {
    case HARD_RESET:
      hard_reset(rx, resp_data, idx);
      break;
    case SOFT_RESET:
      soft_reset(rx, resp_data, idx);
      break;
    case KEY_OFF_ON:
      key_off_on_reset(rx, resp_data, idx);
      break;
    case DISABLE_RAPID_POWER_SHUTDOWN:
      disable_rapid_power_shutdown(rx, resp_data, idx);
      break;
    case ENABLE_RAPID_POWER_SHUTDOWN:
      enable_rapid_power_shutdown(rx, resp_data, idx);
      break;
    default:
      set_failure(rx, resp_data, idx, NRC_SUB_FUNCTION_NOT_SUPPORTED);
      return false;
  }
  return true;
}
