#include "services.h"
#include "../SESSION/state.h"
#include "../UDS/UDS.h"
#include "../UTILS/utils.h"

bool handle_tester_present(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  /** @todo under which condition does this function return false? */
  if (rx->dataLength == 0) {
    return true;
  }
  insertIntoArray(resp_data, rx->data[0], idx);
  return true;
}
