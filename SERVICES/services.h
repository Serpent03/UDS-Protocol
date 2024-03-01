#pragma once

#include "../common.h"
#include "../UDS/UDS.h"

/* ====== UTILITY ====== */

/**
 * @brief Sets the outgoing UDS packet SID and data to report a failure.
 * @param rx The UDS Packet to process.
 * @param resp_data The response data to be edited.
 * @param idx The length of the response data.
 * @param fail_code The failure code of the operation.
 * @return void.
 */
void set_failure(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx, enum NEG_RESPONSE_CODES fail_code);

/**
 * @brief Return a pointer to a UDS Packet after processing data received from the bus.
 * @param rx The UDS Packet to process.
 * @param silenceTx The pointer to the flag that mutes any transmission.
 * @return A pointer to an UDS Packet object.
 */
UDS_Packet* service_handler(UDS_Packet *rx, bool *silenceTx);

/* ====== SERVICES ====== */

/**
 * @brief Wrapper function for the SID 0x10, which is the diagnostic session control.
 * @param rx The UDS packet to process.
 * @param resp_data The returning response data to be edited.
 * @param idx The length of the the response data.
 * @return True if the operation was successful.
 */
bool handle_diag_sess_cntl(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx);

/**
 * @brief Wrapper function for the SID 0x11, which is the ECU reset.
 * @param rx The UDS packet to process.
 * @param resp_data The returning response data.
 * @param idx The length of the response data.
 * @return True if the operation was successful.
 */
bool handle_ecu_reset(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx);



