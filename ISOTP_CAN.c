#include "common.h"
#include "ISOTP_CAN.h"
#include "queue.h"

uInt8 OUT_BUF[8];
uInt8 IN_BUF[8];
uInt8 NULL_BUF[8] = { 0 };
bool INPUT_HAS_CYCLED = true;
FILE *fptr;

void populate_output_buffer(CAN_Frame *cfr) {
  /**
   * For now, just simulate putting the values into the OUT_BUF
   * @todo integrate with the actual routine that deals with the GPIO 
   * @todo utilize FILE streams to simulate GPIO
   **/
  memcpy(OUT_BUF, cfr->data, 8);
  fptr = fopen("GPIO.bin", "ab");
  if (!INPUT_HAS_CYCLED) {
    fwrite(OUT_BUF, sizeof(uInt8), 8, fptr);
  }
  fclose(fptr);
}

void dealloc_CANTP_frame(CAN_Frame *cfr) {
  free(cfr->data);
  free(cfr);
}

void send_ISOTP_frames(UDS_Packet *udsp) {
  queue *data_queue = init_queue(udsp->dataLength + 1);
  enque(data_queue, udsp->SID);
  for (uInt16 i = 0; i < udsp->dataLength; i++) {
    enque(data_queue, udsp->data[i]);
  }

  uInt8 block_size;
  uInt8 STmin;
  uInt8 sequence = 1;

  uInt16 dataLength = len_queue(data_queue);
  CAN_Frame* cfr;
  if (dataLength > 7) {
    /* Each calloc'd CAN_Frame* must be freed after we finish writing data into the OUT_BUF */
    /* First frame in the segmented transmission */
    cfr = CANTP_first_frame(data_queue, dataLength);
    populate_output_buffer(cfr);
    for (uInt8 i = 0; i < 8; i++) {
      printf("%s : 0x%02X\n", i == 0 ? "PCI" : "DAT", OUT_BUF[i]);
    }
    dealloc_CANTP_frame(cfr);
    /** PRE FLOW CONTROL FRAME

     * wait here until we receive the flow control frame 
     * flow control frame sets the block_size and STmin params, which control
     * how often we send the frame and how many in one go
     * if block_size has been init to anything other than 0, we tx
     * that many frames and then return here, waiting for a
     * flow control frame agan.
     * @todo implement interrupts for incoming flow control frame
    */

    /* POST FLOW CONTROL FRAME */
    while (len_queue(data_queue) > 0) {
      printf("\nDLEN: %d\n", len_queue(data_queue));
      cfr = CANTP_consec_frame(data_queue, sequence++);
      populate_output_buffer(cfr);
      for (uInt8 i = 0; i < 8; i++) {
        printf("%s : 0x%02X\n", i == 0 ? "PCI" : "DAT", OUT_BUF[i]);
      }
      dealloc_CANTP_frame(cfr);
    }
  } else {
    /** 
     * @todo 
     * every call after a CANTP_frame() utilizes a send_ISOTP_frames() call
     * all send_ISOTOP_frames() calls utiliz an input/output buffer, with the
     * buffer memory being changed directly through STM32 GPIO
    **/
    cfr = CANTP_single_frame(data_queue, dataLength);
    populate_output_buffer(cfr);
    for (uInt8 i = 0; i < 8; i++) {
      printf("%s : 0x%02X\n", i == 0 ? "PCI" : "DAT", OUT_BUF[i]);
    }
    dealloc_CANTP_frame(cfr);
  }
  free_queue(data_queue);
}

CAN_Frame* CANTP_single_frame(queue* data_queue, uInt16 dataLength) {
  CAN_Frame *cfr = (CAN_Frame*)calloc(1, sizeof(CAN_Frame));
  cfr->data = (uInt8*)calloc(8, sizeof(uInt8));
  uInt8 idx = 0;
  uInt8 queue_data;
  cfr->data[idx++] = (CAN_CODE_SINGLE_FRAME << 4) | (dataLength & 0xF);

  while (idx < 8) {
    /**
     * at_queue_front() returns false if the queue is empty.
     * we load the data into a memory address to get the actual data.
     */
    bool opSuccess = at_queue_front(data_queue, &queue_data);
    cfr->data[idx++] = opSuccess ? queue_data : ISOTP_PADDING;
    dequeue(data_queue);
  }
  return cfr;
}

CAN_Frame* CANTP_first_frame(queue *data_queue, uInt16 dataLength) {
  CAN_Frame *cfr = (CAN_Frame*)calloc(1, sizeof(CAN_Frame));
  cfr->data = (uInt8*)calloc(8, sizeof(uInt8));
  uInt8 idx = 0;
  uInt8 queue_data;

  /** @todo handle this more gracefully than an assert() call to prevent issue in ops */
  assert(dataLength <= 0x1000); /* data must be <= 12 bits; 4096 or 0x1000 */
  uInt16 PCI = (CAN_CODE_FIRST_FRAME << 12) | (dataLength & 0xFFF);
  cfr->data[idx++] = ((PCI >> 8) & 0xFF);
  cfr->data[idx++] = (PCI & 0xFF);

  while (idx < 8) {
    bool opSuccess = at_queue_front(data_queue, &queue_data); 
    cfr->data[idx++] = opSuccess ? queue_data : ISOTP_PADDING;
    dequeue(data_queue);
  }
  return cfr;
}

CAN_Frame* CANTP_consec_frame(queue* data_queue, uInt8 sequenceNum) {
  CAN_Frame* cfr = (CAN_Frame*)calloc(1, sizeof(CAN_Frame));
  cfr->data = (uInt8*)calloc(8, sizeof(uInt8));
  uInt8 idx = 0;
  uInt8 queue_data;
  cfr->data[idx++] = (CAN_CODE_CONSEC_FRAME << 4) | (sequenceNum % 0xF);

  while (idx < 8) {
    bool opSuccess = at_queue_front(data_queue, &queue_data);
    cfr->data[idx++] = opSuccess ? queue_data : ISOTP_PADDING;
    dequeue(data_queue);
  }
  return cfr;
}

bool receive_ISOTP_frames(UDS_Packet *udsp) {
  /**
   * Simulate actual GPIO from FILE stream. INPUT_BUF first gets updated here.
   * First 4 bits of each IN_BUF to verify CAN-TP frame type.
   */
  
  fptr = fopen("GPIO.bin", "rb");
  fread(IN_BUF, sizeof(IN_BUF), sizeof(uInt8), fptr);

  if (memcmp(IN_BUF, NULL_BUF, 8) == 0) {
    return false;
  }

  if (IN_BUF[0] >> 4 == CAN_CODE_SINGLE_FRAME) {
    uInt8 offset = 2; /* The actual data maintains an offset from the start of packet. */
    uInt8 idx = 0;

    udsp->SID = IN_BUF[1];
    udsp->dataLength = (IN_BUF[0] & 0xF) - 1;
    udsp->data = (uInt8*)calloc(udsp->dataLength, sizeof(uInt8));

    for (uInt8 i = 0; i < udsp->dataLength; i++) {
      udsp->data[idx++] = IN_BUF[i + offset];
    }
  } else if (IN_BUF[0] >> 4 == CAN_CODE_FIRST_FRAME){
    /** 
     * Begin the routine for a segmented transmission 
     * First frame should be the CAN-TP First Frame
     * After that we enter a while loop and keep ingesting frames until we 
     * have to send out a flow control frame.
     * INPUT_BUF then gets updated here, until we finish reading the message.
     * @todo Implement the flow control frame on the receiver end.
     */
    uInt8 offset = 3;
    uInt8 idx = 0;

    udsp->SID = IN_BUF[2];
    udsp->dataLength = ((IN_BUF[0] << 8) | IN_BUF[1]) & 0xFFF;
    udsp->data = (uInt8*)calloc(udsp->dataLength, sizeof(uInt8));
    /* FIRST FRAME */
    for (uInt8 i = 0; i < 6; i++) {
      udsp->data[idx++] = IN_BUF[i + offset];
    }
    /* CONSECUTIVE FRAMES */
    offset = 1;
    Int16 byte_num = udsp->dataLength; /* Remaining bytes of data */
    while (byte_num > 0) {
      fread(IN_BUF, sizeof(IN_BUF), sizeof(uInt8), fptr); /* Simulate INPUT pins */
      uInt16 lim = (byte_num < 7) ? byte_num : 7; /* To prevent writing beyond allocated data size. */
      for (uInt8 i = 0; i < lim; i++) {
        udsp->data[idx++] = IN_BUF[i + offset];
      }
      byte_num -= 7;
    }
  } else {
    /* We accept only frames that signify they are the starting of a segmented transmission. 
     * I don't know if this behavior is realistic or not.
     */
    fclose(fptr);
    return false;
  }
  fclose(fptr);
  return true;
}




