#pragma once

#include "../common.h"
#include "session.h"

/**
 * 0 => Used for maintaining information on device diagnostic session.
 * 1 => Status of the security access service.
 * All individual definitions are in the SERVICES/ files.
 */
extern uInt8 DEVICE_STATE[3]; /* All the session states are stored in here. */

enum STATE_CODES {
  STATE_DIAGNOSTIC_SESSION = 0,
  STATE_REQUEST_SERVICE = 1,
};

#define EXTENDED_SESSION_TIMEOUT 5000 
#define SECURITY_REQUEST_TIMEOUT 1000
#define SECURITY_KEY 42 /** @todo generate this randomly */
#define SECURITY_SEED 2 

/**
 * @brief Sets the last communication with the client to the current time.
 * By design, it is called every time the service_handler() in '/SERVICES/services.h' fires, 
 * which means a valid CAN arbitration ID has been detected.
 * @return void.
 */
void set_last_client_call();

/**
 * @brief Sets the session state.
 * @param state The session state to alter.
 * @param code The configuration for the session state.
 * @return void.
 */
void set_state(enum STATE_CODES state, uInt8 code);

/**
 * @brief Gets the session state.
 * @param state The session state to get.
 * @return 1-byte configuration code for the current session state.
 */
uInt8 get_state(enum STATE_CODES state);

/**
 * @brief Called to update the internal server state.
 * This functions as a "maintainer" of sorts, where it sets the relevant states every x or so seconds.
 * For example, this checks if a "tester-present" service has been requested every 5000ms, otherwise
 * it switches from the extended diagnostic session back to the default session.
 */
void update_state();
