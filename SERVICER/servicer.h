#pragma once 

#include "../common.h"
#include "../ISOTP/ISOTP_CAN.h"
#include "../UDS/UDS.h"
#include "timing.h"

#define TX_RETRY_LIMIT 3

extern bool receiveFlag;
extern bool idle;
extern bool transmitFlag;
extern bool processFlag;
extern bool isTransmitter; /** @debug debug flag for testing multiple CAN nodes. */

extern uInt64 CLOCK_TIME_START;
extern uInt64 CLOCK_TIME_CURRENT;
extern uInt64 CLOCK_TIME_OLD;

/**
 * @brief Initializes the server: clocks, flags etc.
 * @returns void.
 */
void Server_Init();

/**
 * @brief Runs the entire server and asssociated services.
 * @returns void.
 */
void Server_Main();

/**
 * @brief Starts the servicer, which provides receiving and transmitting services.
 * @returns void.
 */
void servicer();


