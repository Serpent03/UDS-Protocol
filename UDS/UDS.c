#include "../common.h"
#include "UDS.h"

/* uds_tx will be used for all outbound transmissions. */
UDS_Packet uds_tx;
/* uds_rx will be used for all inbound transmissions. */
UDS_Packet uds_rx;

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
