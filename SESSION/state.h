#include "../common.h"
#include "session.h"

/**
 *
 */
extern uInt8 DEVICE_STATE[2]; /* All the session states are stored in here. */

enum STATE_CODES {
  STATE_DIAG_SESSION = 0,
};

void set_state(enum STATE_CODES state, uInt8 code);
uInt8 get_state(enum STATE_CODES state);
