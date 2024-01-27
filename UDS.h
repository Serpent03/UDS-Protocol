#include "common.h"

enum SID_CODES {
  SID_DIAGNOSTIC_SESS_CNTL = 0x10,
  SID_ECU_RESET = 0x11,
  SID_READ_DATA_BY_IDENT = 0x22,
  SID_READ_DATA_BY_ADDR = 0x23,
  SID_WRITE_DATA_BY_IDENT = 0x2E,
  SID_WRITE_DATA_TO_ADDR = 0x7D
};

// enum SFB_CODES {
//   // only for specific SID_CODES
// };

enum NEG_RESPONSE_CODES {
  // positive response codes => requesting SID + 0x40
  NRC_NEGATIVE_RESPONSE = 0x7F,
  NRC_GENERAL_REJECTION = 0x10,
  NRC_SUB_FUNCTION_NOT_SUPPLIED = 0x12,
  NRC_INCORRECT_MESSAGE_LENGTH = 0x13,
  NRC_BUSY_REPEAT_REQUEST = 0x21,
  NRC_CONDITION_NOT_CORRECT = 0x22,
  NRC_REQUEST_SEQUENCE_ERROR = 0x24,
  NRC_REQUEST_OUT_OF_RANGE = 0x31,
  NRC_SECURITY_ACCESS_DENIED = 0x33,
  NRC_INVALID_KEY = 0x35
};

typedef struct UDS_Packet {
  uInt8 SID;
  uInt8 *data;
  uInt8 dataLength;
} UDS_Packet;

UDS_Packet* generate_UDS_packet(uInt8 SID, uInt8 *data, uInt8 dataLength);
UDS_Packet* parse(UDS_Packet* udsp); // at the server end
