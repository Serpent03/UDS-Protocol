#include "../common.h"
#include "../ISOTP/ISOTP_CAN.h"
#include "../QUEUE/queue.h"
#include "../SERVICER/timing.h"
#include "../BUS/bus.h"

uInt8 NULL_BUF[10] = { 0 };
uInt8 OUT_BUF[10];
uInt8 IN_BUF[10];
uInt8 *CAN_DATA = &IN_BUF[2];
/* The first two bytes from input will be the address + DLC. We only care about the CAN data transmitted,
 * so we will refer to it using a pointer located on the third byte of the input buffer */

bool INPUT_HAS_CYCLED = true;
FILE *fptr;
FILE *fcfptr; /* FILE ptr for the flow control frame */

ISO_TP_Frame ITFR_TX;
ISO_TP_Frame ITFR_FC; /* I think I can probably do without this, will have to see. */

uInt8 block_size_recv = 0;
uInt8 STmin_recv = 0;
bool FC_INIT = true;

uInt8 block_size_send = 2;
uInt8 STMin_send = 0;
bool FC_SEND = false;

void print_OUTBUF() {
  printf("ADDR: 0x%02X\n", (uInt16)((OUT_BUF[0] << 8) | OUT_BUF[1]) >> 5);
  printf("_DLC: 0x%03X\n", OUT_BUF[1] & 0xF);
  for (uInt8 i = 2; i < 10; i++) {
    printf("%s : 0x%02X\n", i == 2 ? "PCI" : "DAT", OUT_BUF[i]);
  }
}

void populate_output_buffer(ISO_TP_Frame *ITFR) {
  /**
   * @todo call into BUS.h here.
   **/
  OUT_BUF[0] = (ITFR->addr >> 8);
  OUT_BUF[1] = (ITFR->addr);
  memcpy(&OUT_BUF[2], ITFR->data, 8);
  write_to_bus(OUT_BUF, sizeof(OUT_BUF));
}

bool send_ISOTP_frames(UDS_Packet *udsp, uInt16 from_addr) {
  queue *data_queue = init_queue(udsp->dataLength + 1);
  enque(data_queue, udsp->SID);
  for (uInt16 i = 0; i < udsp->dataLength; i++) {
    enque(data_queue, udsp->data[i]);
  }

  /* 
   * The address is 11-bits. Assuming last 5 bits can be used 
   * for the DLC setting by some bit-shifting magic. 
   */
  uInt16 sequence = 1;
  uInt16 dataLength = len_queue(data_queue);
  ITFR_TX.addr = ((from_addr << 5) | DEFAULT_DLC);
  setTime(&CLOCK_TIME_AT_TX);


  if (dataLength > 7) {
    /* First frame in the segmented transmission. */

    /**
     * @todo FF bits for sending messages more than 4096 bytes long.
     */
    if (!CANTP_first_frame(&ITFR_TX, data_queue, dataLength)) {
      return false;
    }
    populate_output_buffer(&ITFR_TX);
    print_OUTBUF();
    /*
     * wait here until we receive the flow control frame 
     * flow control frame sets the block_size and STmin params
    */

    /* Set up the STMin, BS params through GPI.bin */
    while (!FC_INIT) {
      FC_INIT = CANTP_read_flow_control_frame();
      if (!check_if_timeout(CLOCK_TIME_AT_TX, ISOTP_N_Bs)) {
        return false;
      }
      // FC_INIT = true;
    }

    /** 
     * POST FLOW CONTROL FRAME 
     * @todo Utilization of N_Cs 
     */
    while (len_queue(data_queue) > 0) {
      if (FC_INIT && block_size_recv > 0) {
        /* This should decrement BS if we're not on unlimited block-sizes(BS = 0), i.e 
         * sending blocks until our data is all sent. */
        block_size_recv--;
        FC_INIT = block_size_recv > 0;
      }
      while (!FC_INIT) {
        FC_INIT = CANTP_read_flow_control_frame();
        if (!check_if_timeout(CLOCK_TIME_AT_TX, ISOTP_N_Bs)) {
          return false;
        }
        // block_size_recv = 1;
      }
      printf("\nDLEN: %d\n", len_queue(data_queue));
      if (!CANTP_consec_frame(&ITFR_TX, data_queue, sequence++)) {
        return false;
      }
      populate_output_buffer(&ITFR_TX);
      print_OUTBUF();
    }
  } else {
    if (!CANTP_single_frame(&ITFR_TX, data_queue, dataLength)) {
      return false;
    }
    populate_output_buffer(&ITFR_TX);
    print_OUTBUF();
  }
  return true;
}

bool CANTP_single_frame(ISO_TP_Frame *ITFR, queue* data_queue, uInt16 dataLength) {
  uInt8 idx = 0;
  uInt8 queue_data;
  if (dataLength == 0) {
    return false;
  }
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
  return true;
}

bool CANTP_first_frame(ISO_TP_Frame *ITFR, queue *data_queue, uInt16 dataLength) {
  uInt8 idx = 0;
  uInt8 queue_data;

  if (dataLength >= 0x1000){ /* data must be <= 12 bits; 4096 or 0x1000 */
    return false;
  }   
  
  uInt16 PCI = (CAN_CODE_FIRST_FRAME << 12) | (dataLength & 0xFFF);
  ITFR->data[idx++] = ((PCI >> 8) & 0xFF);
  ITFR->data[idx++] = (PCI & 0xFF);

  while (idx < 8) {
    bool opSuccess = at_queue_front(data_queue, &queue_data); 
    ITFR->data[idx++] = opSuccess ? queue_data : ISOTP_PADDING;
    dequeue(data_queue);
  }
  /* If the transmission has happened in time limits, this will return true. */
  return true && check_if_timeout(CLOCK_TIME_AT_TX, ISOTP_N_As);
}

bool CANTP_consec_frame(ISO_TP_Frame *ITFR, queue* data_queue, uInt16 sequenceNum) {
  uInt8 idx = 0;
  uInt8 queue_data;
  ITFR->data[idx++] = (CAN_CODE_CONSEC_FRAME << 4) | (sequenceNum % 0xF);

  while (idx < 8) {
    bool opSuccess = at_queue_front(data_queue, &queue_data);
    ITFR->data[idx++] = opSuccess ? queue_data : ISOTP_PADDING;
    dequeue(data_queue);
  }
  return true && check_if_timeout(CLOCK_TIME_AT_TX, ISOTP_N_As);
}

bool CANTP_read_flow_control_frame() {
  // printf("%lu :: %lu\n", CLOCK_TIME_AT_TX + TX_TIME_LIMIT, getTime());
  printf("\nREADING FLOW CONTROL FRAME\n");
  if (!read_from_bus(IN_BUF, sizeof(IN_BUF))) {
    return false;
  }
  if (memcmp(IN_BUF, NULL_BUF, 10) == 0) {
    return false;
  } else if ((CAN_DATA[0] >> 4) != CAN_CODE_FLOW_CNTL_FRAME) {
    return false;
  }
  block_size_recv = CAN_DATA[1];
  STmin_recv = CAN_DATA[2];

  return true;
}

bool CANTP_write_flow_control_frame(uInt16 addr) {
  printf("\nWRITING FLOW CONTROL FRAME\n");
  
  ITFR_FC.addr = ((addr << 5) | DEFAULT_DLC);
  ITFR_FC.data[0] = (CAN_CODE_FLOW_CNTL_FRAME << 4) | (CAN_FC_FLAG_0);
  ITFR_FC.data[1] = block_size_send;
  ITFR_FC.data[2] = STMin_send;
  populate_output_buffer(&ITFR_FC);

  return true;
}

bool receive_ISOTP_frames(UDS_Packet *udsp, uInt16 tx_addr) {
  /**
   * Simulate actual GPIO from FILE stream. INPUT_BUF first gets updated here.
   * First 4 bits of each IN_BUF to verify CAN-TP frame type.
   */
  
  setTime(&CLOCK_TIME_AT_RX);

  if (!read_from_bus(IN_BUF, sizeof(IN_BUF))) {
    return false;
  }
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
     */
    uInt8 offset = 3;
    uInt16 idx = 0;

    udsp->SID = CAN_DATA[2];
    udsp->dataLength = (((CAN_DATA[0] << 8) | CAN_DATA[1]) & 0xFFF) - 1;
    /* FIRST FRAME */
    for (uInt8 i = 0; i < 6; i++) {
      udsp->data[idx++] = CAN_DATA[i + offset];
    }
    idx--;
    /* We decrement the idx and udsp->dataLength, as the CAN packet accounted for the SID to be IN 
     * the data itself -- which is not the case. */

    /** @todo Verify necessity of N_Br */
    while (!FC_SEND) {
      FC_SEND = CANTP_write_flow_control_frame(tx_addr);
      if (!check_if_timeout(CLOCK_TIME_AT_RX, ISOTP_N_Ar)) {
        return false;
      }
      // FC_SEND = true;
    }
    uInt8 block_size_copy = block_size_send;
    /* We'll maintain this copy to know when we have to send block_size information again. */


    /* CONSECUTIVE FRAMES */
    offset = 1;
    Int16 byte_num = udsp->dataLength; /* Remaining bytes of data */

    /** @todo Utilization of N_Cr */
    while (byte_num > 0) {
      if (FC_SEND && block_size_copy > 0) {
        block_size_copy--;
        FC_SEND = block_size_copy > 0;
      }
      while (!FC_SEND) {
        FC_SEND = CANTP_write_flow_control_frame(tx_addr);
        block_size_copy = block_size_send;
        if (!check_if_timeout(CLOCK_TIME_AT_RX, ISOTP_N_Ar)) {
          return false;
        }
      }
      /* We'll only have to send the FC frame if our block size is anything else than 0. So once we see that it is 0,
       * we will have to send the FC frame again, which is what the FC_SEND boolean is for. */

      read_from_bus(IN_BUF, sizeof(IN_BUF));
      uInt16 lim = (byte_num < 7) ? byte_num : 7; /* To prevent writing beyond allocated data size. */
      for (uInt16 i = 0; i < lim; i++) {
        udsp->data[idx++] = CAN_DATA[i + offset];
      }
      byte_num -= 7;
    }
  } else {
    /* We accept only frames that signify they are the starting of a segmented transmission. 
     * I don't know if this behavior is realistic or not.
     */
    return false;
  }
  return true;
}




