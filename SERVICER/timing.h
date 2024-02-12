#pragma once

#include "../common.h"
#include <sys/time.h>

#define CLOCK_CYCLE 200

/**
 * A -> sending first or single 
 * B -> rx frames
 * C -> consec frame
*/
enum ISO_TP_TIME_LIMITS {
  ISOTP_N_As = 1000,
  ISOTP_N_Bs = 1000,
  ISOTP_N_Cs = 1000,

  ISOTP_N_Ar = 1000,
  ISOTP_N_Br = 1000,
  ISOTP_N_Cr = 1000,
};

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

/**
 * @brief Used to check if an activity has happened within time limits.
 * @param CLOCK_TIME The UNIX timestamp to be used for reference.
 * @param time_limit The time limit to check.
 * @returns True if the time limit has not been exceeded.
 * */
bool check_if_timeout(uInt64 CLOCK_TIME, enum ISO_TP_TIME_LIMITS time_limit);
