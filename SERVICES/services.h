#pragma once

#include "../common.h"
#include "../UDS/UDS.h"

/**
 * @brief Return a pointer to a UDS Packet after processing data received from the bus.
 * @param rx The UDS Packet to process.
 * @param silenceTx The pointer to the flag that mutes any transmission.
 * @return A pointer to an UDS Packet object.
 */
UDS_Packet* service_handler(UDS_Packet *rx, bool *silenceTx);

/* All the specific service modes start from here. Each has handle_ appended in front of it for the sake of naming convention. */

/**
 * @brief Wrapper function for the SID 0x10, which is the diagnostic session control.
 * @param rx The UDS packet to process.
 * @param resp_data The returning response data to be edited.
 * @param idx The length of the the response data.
 * @return True if the operation was successful.
 */
bool handle_diag_sess_cntl(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx);



