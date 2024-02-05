#pragma once 

#include "common.h"

enum SID_CODES {
  SID_DIAGNOSTIC_SESS_CNTL = 0x10,
  SID_ECU_RESET = 0x11,
  SID_READ_DATA_BY_IDENT = 0x22,
  SID_READ_DATA_BY_ADDR = 0x23,
  SID_WRITE_DATA_BY_IDENT = 0x2E,
  SID_WRITE_DATA_TO_ADDR = 0x7D
};

enum SFB_CODES {
  SFB_TESTER_PRESENT = 0x00,
  SFB_READ_DTC_INFORMATION = 0x02,
  SFB_ECU_RESET = 0x01,
  SFB_LINK_CNTL_1 = 0x01,
  SFB_LINK_CNTL_2 = 0x02,
  SFB_LINK_CNTL_3 = 0x03,
};

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
  NRC_INVALID_KEY = 0x35,
  NRC_ENGINE_RUNNING = 0x83,
  NRC_SPEED_TOO_HIGH = 0x88
};

typedef struct UDS_Packet {
  uInt8 SID;
  uInt8 data[4096];
  uInt16 dataLength;
} UDS_Packet;

extern UDS_Packet uds_tx;
extern UDS_Packet uds_rx;
extern uInt8 resp_data[4096];

/**
 * @brief Generates a UDS packet.
 * @param SID The required SID code for the operation.
 * @param data Pointer to a uInt8 array containing relevant UDS data.
 * @param dataLen Length of the array containing UDS data.
 * @return void.
 */
UDS_Packet* generate_UDS_packet(uInt8 SID, uInt8 *data, uInt16 dataLength);

/**
 * @brief Perform an operation on the UDS packet we have received.
 * @param udsp The received UDS packet to perform an operation on.
 * @return An UDS packet to be sent as a reply.
 */
UDS_Packet* parse(UDS_Packet* udsp); // at the server end
