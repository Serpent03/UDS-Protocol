#pragma once 

#include "../common.h"
#include "../ISOTP/ISOTP_CAN.h"
#include "../UDS/UDS.h"
#include "timing.h"

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
 * @return void.
 */
void Server_Init();

/**
 * @brief Runs the entire server and asssociated services.
 * @return void.
 */
void Server_Main();

/**
 * @brief Starts the servicer, which provides receiving and transmitting services.
 * @return void.
 */
void servicer();


