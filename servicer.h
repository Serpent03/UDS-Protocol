#pragma once 

#include "common.h"
#include "ISOTP_CAN.h"
#include "UDS.h"

#define CLOCK_CYCLE 1000

extern bool receiveFlag;
extern bool idle;
extern bool transmitFlag;

extern uInt64 CLOCK_TIME_START;
extern uInt64 CLOCK_TIME_CURRENT;
extern uInt64 CLOCK_TIME_OLD;

/**
 * @brief Initializes the server.
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


