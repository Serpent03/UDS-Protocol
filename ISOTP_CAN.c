#include "common.h"
#include "ISOTP_CAN.h"
#include "queue.h"

uInt8 OUT_BUF[8];
uInt8 IN_BUF[8];

void populate_output_buffer(CAN_Frame *cfr) {
  /**
   * For now, just simulate putting the values into the OUT_BUF
   * @todo integrate with the actual routine that deals with the GPIO 
   **/
  memcpy(OUT_BUF, cfr->data, 8);
}

void send_ISOTP_frames(UDS_Packet *udsp) {
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
    /** 
    * @todo 
    * every call after a CANTP_frame() utilizes a send_ISOTP_frames() call
    * all send_ISOTOP_frames() calls utiliz an input/output buffer, with the
    * buffer memory being changed directly through STM32 GPIO
    **/
    populate_output_buffer(cfr);
    for (uInt8 i = 0; i < 8; i++) {
      printf("%s : 0x%02X\n", i == 0 ? "PCI" : "DAT", OUT_BUF[i]);
    }
  }
  free_queue(data_queue);
}

CAN_Frame* single_CANTP_frame(queue* data_queue, uInt16 dataLength) {
  CAN_Frame *cfr = (CAN_Frame*)calloc(1, sizeof(CAN_Frame));
  cfr->data = (uInt8*)calloc(8, sizeof(uInt8));
  uInt8 idx = 0;
  uInt8 data;
  cfr->data[idx++] = (CAN_CODE_CONSEC_FRAME << 4) | (dataLength & 0xF);

  while (idx < 8) {
    // at_queue_front() returns false if the queue is empty.
    // we load the data into a memory address to get the actual data.
    bool opSuccess = at_queue_front(data_queue, &data);
    cfr->data[idx++] = opSuccess ? data : ISOTP_PADDING;
    dequeue(data_queue);
  }
  return cfr;
}
