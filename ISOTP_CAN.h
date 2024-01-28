#include "common.h"
#include "UDS.h"
#include "queue.h"

#define ISOTP_PADDING 0xAA

enum CAN_FRAME_CODES {
  CAN_CODE_SINGLE_FRAME = 0x0,
  CAN_CODE_FIRST_FRAME = 0x1,
  CAN_CODE_CONSEC_FRAME = 0x2,
  CAN_CODE_FLOW_CNTL_FRAME = 0x3 
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

typedef struct ISO_TP_Frame {
  uInt16 addr;
  CAN_Frame *cfr; 
} ISO_TP_Frame;

enum defaultAddress {
  DEFAULT_RX_ADDR = 0x6789,
  DEFAULT_TX_ADDR = 0x1234,
};
// TODO set input and output buffer 

CAN_Frame* single_CANTP_frame(queue* data_queue, uInt16 dataLength);
CAN_Frame* multiple_CANTP_frame(queue* data_queue);

void generate_ISOTP_frames(UDS_Packet *udsp);
void send_ISOTP_frames(CAN_Frame* cfr);

