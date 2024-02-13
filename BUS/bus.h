#pragma once

#include "../common.h"

/** 
 * @todo Implement client-server session. 
 * This has to be initialized at first contact.
 * FILE I/O can be utilized to simulate the communication bus,
 * where each process only picks up messages meant for it.
 */

typedef struct SESSION {
  uInt16 CLIENT_ADDR;
  uInt16 SERVER_ADDR;
} SESSION;

/** @todo I/O triggers can be simulated here. */


