#include "../../common.h"
#include "../../UDS/UDS.h"
#include "../services.h"

/**
 * @brief Wrapper function for the SID 0x22, which is read data by id.
 * @param rx The UDS packet to process.
 * @param resp_data The returning response data.
 * @param idx The length of the response data.
 * @return True if the operation was successful.
 */
bool handle_read_data_by_id(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx);
