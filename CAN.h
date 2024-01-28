#include "common.h"
#include "UDS.h"

enum CAN_FRAME_CODES {
  CAN_SINGLE_FRAME_CODE = 0x0,
  CAN_FIRST_FRAME_CODE = 0x1,
  CAN_CONSEC_FRAME_CODE = 0x2,
  CAN_FLOW_CNTL_FRAME_CODE = 0x3 
};

enum CAN_FRAME_TYPE {
  CAN_TYPE_FLOW_CONTROL,
  CAN_TYPE_SINGLE_FRAME,
  CAN_TYPE_FIRST_FRAME,
  CAN_TYPE_CONSEC_FRAME
};

typedef struct CAN_Frame {
  // this can only be 8 bytes long everytime. Either segment or pad!
  uInt8 *PCI;
  uInt8 *data;
} CAN_Frame;

typedef struct ISO_TP {
  uInt16 addr;
  CAN_Frame *cfr; 
} ISO_TP;

CAN_Frame* generate_CAN_packet(enum CAN_FRAME_TYPE cft, UDS_Packet* udsp);
void generate_ISOTP_frames(UDS_Packet *udsp);
