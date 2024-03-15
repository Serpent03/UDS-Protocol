#pragma once

#include "../common.h"
#include "../UDS/UDS.h"

#define NUM_SERVICES 5

/* ====== STRUCTURES ====== */
  
typedef struct service_functions {
  uInt8 SID;
  bool (*callback_function)(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx);
  uInt8 diag_sess;
  uInt8 security_level;
} service_functions;

extern service_functions service_table[NUM_SERVICES];

/* ====== UTILITY ====== */

/**
 * @brief Set up the service table for referencing the SID wrapper functions.
 * The service table needs to be set up with all implemented UDS routines otherwise
 * they will be unreachable from the event loop.
 * @return void.
 */
void init_service_table();

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
 * @brief Wrapper function for the subroutine, which returns the state of the internal variables.
 * @param rx The UDS Packet to process.
 * @param resp_data The returning response data to be edited.
 * @param idx The length of the response data.
 * @return void.
 */
void handle_debug(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx);

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

/**
 * @brief Wrapper function for the SID 0x27, which is the security access service.
 * @param rx The UDS packet to process.
 * @param resp_data The returning response data.
 * @param idx The length of the response data.
 * @return True if the operation was successful.
 */
bool handle_security_access(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx);

/**
 * @brief Wrapper function for the SID 0x3E, which is the tester present.
 * @param rx The UDS packet to process.
 * @param resp_data The returning response data.
 * @param idx The length of the response data.
 * @return True if the operation was successful.
 */
bool handle_tester_present(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx);




