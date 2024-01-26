#include <stdio.h>
#include <stdlib.h>

enum SID_CODES {
  DIAGNOSTIC_SESS_CNTL = 0x10,
  ECU_RESET = 0x11,
  READ_DATA_BY_IDENT = 0x22,
  READ_DATA_BY_ADDR = 0x23,
  WRITE_DATA_BY_IDENT = 0x2E,
  WRITE_DATA_TO_ADDR = 0x7D
};

enum RESPONSE_CODES {
  // other response codes could just be SID_CODES + 0x40
  NEGATIVE_RESPONSE = 0x7F
};
