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


