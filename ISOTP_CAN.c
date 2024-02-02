#include "common.h"
#include "ISOTP_CAN.h"
#include "queue.h"

uInt8 NULL_BUF[10] = { 0 };
uInt8 OUT_BUF[10];
uInt8 IN_BUF[10];
uInt8 *CAN_DATA = &IN_BUF[2];
/* The first two bytes from input will be the address + DLC. We only care about the CAN data transmitted,
 * so we will refer to it using a pointer located on the third byte of the input buffer */

bool INPUT_HAS_CYCLED = true;
FILE *fptr;

ISO_TP_Frame ITFR_TX;
ISO_TP_Frame ITFR_FC; /* I think I can probably do without this, will have to see. */

uInt8 block_size = -1;
uInt8 STmin = -1;
bool FC_INIT = false;

void print_OUTBUF() {
  printf("ADDR: 0x%02X\n", (uInt16)((OUT_BUF[0] << 8) | OUT_BUF[1]) >> 5);
  printf("_DLC: 0x%03X\n", OUT_BUF[1] & 0xF);
  for (uInt8 i = 2; i < 10; i++) {
    printf("%s : 0x%02X\n", i == 2 ? "PCI" : "DAT", OUT_BUF[i]);
  }
}

void populate_output_buffer(ISO_TP_Frame *ITFR) {
  /**
   * For now, just simulate putting the values into the OUT_BUF
   * @todo integrate with the actual routine that deals with the GPIO 
   **/
  OUT_BUF[0] = (ITFR->addr >> 8);
  OUT_BUF[1] = (ITFR->addr);
  memcpy(&OUT_BUF[2], ITFR->data, 8);
  if (!INPUT_HAS_CYCLED) {
    fptr = fopen("GPO.bin", "ab");
    fwrite(OUT_BUF, sizeof(uInt8), 10, fptr);
    fclose(fptr);
  }
}

void send_ISOTP_frames(UDS_Packet *udsp, uInt16 rx_addr) {
  queue *data_queue = init_queue(udsp->dataLength + 1);
  enque(data_queue, udsp->SID);
  for (uInt16 i = 0; i < udsp->dataLength; i++) {
    enque(data_queue, udsp->data[i]);
  }

  uInt8 sequence = 1;

  uInt16 dataLength = len_queue(data_queue);
  ITFR_TX.addr = ((rx_addr << 5) | DEFAULT_DLC);
  /* 
   * The address is 11-bits. Assuming last 5 bits can be used 
   * for the DLC setting by some bit-shifting magic. 
   */

  if (dataLength > 7) {
    /* First frame in the segmented transmission */
    CANTP_first_frame(&ITFR_TX, data_queue, dataLength);
    populate_output_buffer(&ITFR_TX);
    print_OUTBUF();
    /** PRE FLOW CONTROL FRAME

     * wait here until we receive the flow control frame 
     * flow control frame sets the block_size and STmin params, which control
     * how often we send the frame and how many in one go
     * if block_size has been init to anything other than 0, we tx
     * that many frames and then return here, waiting for a
     * flow control frame agan.
     * @todo implement interrupts for incoming flow control frame
    */

    /* Set up the STMin, BS params through GPI.bin */
    while (!FC_INIT) {
      FC_INIT = CANTP_read_flow_control_frame();
      FC_INIT = true;
      /** @todo Add in time-out abort waiting for FC-frame */
    }

    /* POST FLOW CONTROL FRAME */
    while (len_queue(data_queue) > 0) {
      if (FC_INIT && block_size > 0) {
        /* This should only trigger if we've initialized BS > 0 */
        block_size--;
        FC_INIT = block_size > 0;
      }
      while (!FC_INIT) {
        FC_INIT = CANTP_read_flow_control_frame();
      }
      printf("\nDLEN: %d\n", len_queue(data_queue));
      CANTP_consec_frame(&ITFR_TX, data_queue, sequence++);
      populate_output_buffer(&ITFR_TX);
      print_OUTBUF();
    }
  } else {
    CANTP_single_frame(&ITFR_TX, data_queue, dataLength);
    populate_output_buffer(&ITFR_TX);

    print_OUTBUF();
  }
}

void CANTP_single_frame(ISO_TP_Frame *ITFR, queue* data_queue, uInt16 dataLength) {
  uInt8 idx = 0;
  uInt8 queue_data;
  ITFR->data[idx++] = (CAN_CODE_SINGLE_FRAME << 4) | (dataLength & 0xF);

  while (idx < 8) {
    /**
     * at_queue_front() returns false if the queue is empty.
     * we load the data into a memory address to get the actual data.
     */
    bool opSuccess = at_queue_front(data_queue, &queue_data);
    ITFR->data[idx++] = opSuccess ? queue_data : ISOTP_PADDING;
    dequeue(data_queue);
  }
}

void CANTP_first_frame(ISO_TP_Frame *ITFR, queue *data_queue, uInt16 dataLength) {
  uInt8 idx = 0;
  uInt8 queue_data;

  /** @todo handle this more gracefully than an assert() call to prevent issue in ops */
  assert(dataLength <= 0x1000); /* data must be <= 12 bits; 4096 or 0x1000 */
  uInt16 PCI = (CAN_CODE_FIRST_FRAME << 12) | (dataLength & 0xFFF);
  ITFR->data[idx++] = ((PCI >> 8) & 0xFF);
  ITFR->data[idx++] = (PCI & 0xFF);

  while (idx < 8) {
    bool opSuccess = at_queue_front(data_queue, &queue_data); 
    ITFR->data[idx++] = opSuccess ? queue_data : ISOTP_PADDING;
    dequeue(data_queue);
  }
}

void CANTP_consec_frame(ISO_TP_Frame *ITFR, queue* data_queue, uInt8 sequenceNum) {
  uInt8 idx = 0;
  uInt8 queue_data;
  ITFR->data[idx++] = (CAN_CODE_CONSEC_FRAME << 4) | (sequenceNum % 0xF);

  while (idx < 8) {
    bool opSuccess = at_queue_front(data_queue, &queue_data);
    ITFR->data[idx++] = opSuccess ? queue_data : ISOTP_PADDING;
    dequeue(data_queue);
  }
}

bool CANTP_read_flow_control_frame() {
  /* Read the FC params - STMin and BS. */
  printf("\nFLOW CONTROL FRAME\n");
  fptr = fopen("GPI.bin", "rb");
  if (fptr == NULL) {
    return false;
  }
  fread(IN_BUF, sizeof(IN_BUF), sizeof(uInt8), fptr);

  if (memcmp(IN_BUF, NULL_BUF, 10) == 0) {
    return false;
  } else if ((CAN_DATA[0] >> 4) != CAN_CODE_FLOW_CNTL_FRAME) {
    return false;
  }

  block_size = CAN_DATA[1];
  STmin = CAN_DATA[2];

  return true;
}

bool receive_ISOTP_frames(UDS_Packet *udsp) {
  /**
   * Simulate actual GPIO from FILE stream. INPUT_BUF first gets updated here.
   * First 4 bits of each IN_BUF to verify CAN-TP frame type.
   */
  
  fptr = fopen("GPI.bin", "rb");
  if (fptr == NULL) {
    return false;
  }
  fread(IN_BUF, sizeof(IN_BUF), sizeof(uInt8), fptr);
  if (memcmp(IN_BUF, NULL_BUF, 10) == 0) {
    return false;
  } 

  if (CAN_DATA[0] >> 4 == CAN_CODE_SINGLE_FRAME) {
    uInt8 offset = 2; /* The actual data maintains an offset from the start of packet. */
    uInt8 idx = 0;

    udsp->SID = CAN_DATA[1];
    udsp->dataLength = (CAN_DATA[0] & 0xF) - 1;
    // udsp->data = (uInt8*)calloc(udsp->dataLength, sizeof(uInt8));

    for (uInt8 i = 0; i < udsp->dataLength; i++) {
      udsp->data[idx++] = CAN_DATA[i + offset];
    }
  } else if (CAN_DATA[0] >> 4 == CAN_CODE_FIRST_FRAME){
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

    udsp->SID = CAN_DATA[2];
    udsp->dataLength = ((CAN_DATA[0] << 8) | CAN_DATA[1]) & 0xFFF;
    // udsp->data = (uInt8*)calloc(udsp->dataLength, sizeof(uInt8));
    /* FIRST FRAME */
    for (uInt8 i = 0; i < 6; i++) {
      udsp->data[idx++] = CAN_DATA[i + offset];
    }
    /* CONSECUTIVE FRAMES */
    offset = 1;
    Int16 byte_num = udsp->dataLength; /* Remaining bytes of data */
    while (byte_num > 0) {
      fread(IN_BUF, sizeof(IN_BUF), sizeof(uInt8), fptr); /* Simulate INPUT pins */
      uInt16 lim = (byte_num < 7) ? byte_num : 7; /* To prevent writing beyond allocated data size. */
      for (uInt8 i = 0; i < lim; i++) {
        udsp->data[idx++] = CAN_DATA[i + offset];
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




