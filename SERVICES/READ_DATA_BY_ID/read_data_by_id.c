/**
 * @todo Implement the RDBID SID.
 * It takes the DID in the UDS packet,
 * and returns data.
 * 
 * @brief The Read Data By Identifier (0x22) takes in a two-byte identifier
 * which then is fed into the HAL layer function to read some address in the memory.
 * The resulting message is then pushed into the UDS packet and transmitted
 * back to the original sender.
 * @param DID The identity of the location to read from the HAL layer.
 * @param LEN How much bytes to read.
 * @return The data at that location in the UDS packet.
 */

#include "../../common.h"
#include "../../UDS/UDS.h"
#include "read_data_by_id.h"


/**
 * @todo create stub functions for reading data.
 */

bool handle_read_data_by_id(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx) {
  return true;
}
