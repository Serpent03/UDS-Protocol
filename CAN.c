#include "common.h"
#include "CAN.h"
#include "queue.h"


void generate_ISOTP_frames(UDS_Packet *udsp) {
  // put data inside a queue.
  // reference queue to this helper function
  queue *data_queue = init_queue(udsp->dataLength + 1);
  enque(data_queue, udsp->SID);
  for (int i = 0; i < udsp->dataLength; i++) {
    enque(data_queue, udsp->data[i]);
  }
  if (len_queue(data_queue) > 7) {
    // follow the segmentation protocol
  } else {
    // single frame!
  }
  free_queue(data_queue);
}

CAN_Frame* generate_CAN_packet(enum CAN_FRAME_TYPE cft, UDS_Packet *udsp) {
  // single frame: data < 8B
  // first frame: data >= 8B
  // flow control: rx setting rates
  // consecutive frame: send actual data

  uInt8 *data;
  uInt8 d_idx = 0;
  uInt8 d_lim;

  uInt8 *pci;
  uInt8 p_idx = 0;
  uInt8 p_lim;

  switch (cft) {
    case CAN_TYPE_SINGLE_FRAME:
      data = (uInt8*)calloc(7, sizeof(uInt8));
      d_lim = udsp->dataLength + 1;
      pci = (uInt8*)calloc(1, sizeof(uInt8));
      p_lim = 1;
      break;
    case CAN_TYPE_CONSEC_FRAME:
      data = (uInt8*)calloc(7, sizeof(uInt8));
      d_lim = 7;
      pci = (uInt8*)calloc(1, sizeof(uInt8));
      p_lim = 1;
    case CAN_TYPE_FLOW_CONTROL:
      data = (uInt8*)calloc(5, sizeof(uInt8));
      d_lim = 5;
      pci = (uInt8*)calloc(3, sizeof(uInt8));
      p_lim = 3;
    case CAN_TYPE_FIRST_FRAME:
      data = (uInt8*)calloc(6, sizeof(uInt8));
      d_lim = 6;
      pci = (uInt8*)calloc(2, sizeof(uInt8));
      p_lim = 2;
    default:
      break;
  }
  // now here is the tough part: segmentation
  // queue based?
}

