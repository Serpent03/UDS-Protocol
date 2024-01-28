#include "common.h"
#include "ISOTP_CAN.h"
#include "queue.h"

uInt8 OUT_BUF[8];
uInt8 IN_BUF[8];

void generate_ISOTP_frames(UDS_Packet *udsp) {
  // put data inside a queue.
  // reference queue to this helper function
  queue *data_queue = init_queue(udsp->dataLength + 1);
  enque(data_queue, udsp->SID);
  for (uInt16 i = 0; i < udsp->dataLength; i++) {
    enque(data_queue, udsp->data[i]);
  }
  uInt16 dataLength = len_queue(data_queue);
  if (dataLength > 7) {
    // generate_multiple_frame(queue)
    // -> generate(first_frame)
    // -> wait for (control flow)
    // -> send rest of the frames
  } else {
    // generate_single_frame
    // append each frame inside the ISO_TP_Frame with addr
    CAN_Frame* cfr = single_CANTP_frame(data_queue, dataLength);
    printf("PCI: 0x%02x\n", cfr->PCI[0]);
    for (uInt16 i = 0; i < 7; i++) {
      printf("DAT: 0x%02x\n", cfr->data[i]);
    }
    /*** 
    * @todo 
    * every call after a CANTP_frame() utilizes a send_ISOTP_frames() call
    * all send_ISOTOP_frames() calls utiliz an input/output buffer, with the
    * buffer memory being changed directly through STM32 GPIO
    * ***/
    // send_ISOTP_frames(cfr);
  }
  free_queue(data_queue);
}

CAN_Frame* single_CANTP_frame(queue* data_queue, uInt16 dataLength) {
  CAN_Frame *cfr = (CAN_Frame*)calloc(1, sizeof(CAN_Frame));
  cfr->PCI = (uInt8*)calloc(1, sizeof(uInt8));
  cfr->PCI[0] = (CAN_CODE_CONSEC_FRAME << 4) | (dataLength & 0xF);

  cfr->data = (uInt8*)calloc(7, sizeof(uInt8));
  uInt8 idx = 0;
  uInt8 data;
  while (idx < 7) {
    // at_queue_front() returns false if the queue is empty.
    // we load the data into a memory address to get the actual data.
    bool opSuccess = at_queue_front(data_queue, &data);
    cfr->data[idx] = opSuccess ? data : ISOTP_PADDING;
    dequeue(data_queue);
    idx++;
  }
  return cfr;
}
