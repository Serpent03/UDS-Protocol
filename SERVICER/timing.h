#pragma once

#include "../common.h"
#include <sys/time.h>

#define TX_TIME_LIMIT 150 /* The transmitting message should happen in 150ms */
#define CLOCK_CYCLE 200

extern uInt64 CLOCK_TIME_AT_TX;
extern uInt64 CLOCK_TIME_AT_RX;

/**
 * @brief Gets the current UNIX timestamp.
 * @returns Current UNIX timestamp.
 */
uInt64 getTime();

/**
 * @brief Sets the current UNIX timestamp inside the passed variable.
 * @param CLOCK_VAR The 64-bit unsigned integer to put the timestamp in.
 * @returns void.
 */
void setTime(uInt64 *VAR);


