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

// enum SFB_CODES {
//   // only for specific SID_CODES
// };

enum RESPONSE_CODES {
  // other response codes could just be SID_CODES + 0x40
  NEGATIVE_RESPONSE = 0x7F
};

enum UDS_PACKET_TYPE {
  REQUEST,
  POSRESP,
  NEGRESP
};

typedef struct UDS_Packet {
  unsigned char param_1;
  unsigned char param_2;
  unsigned short int payload_1;
  unsigned short int payload_2;
} UDS_Packet;


UDS_Packet* generate_UDS_packet(enum UDS_PACKET_TYPE packetType, unsigned char param1, unsigned char param2, unsigned short int payload_1, unsigned short int payload_2);
UDS_Packet* parse(UDS_Packet* udsp); // at the server end


