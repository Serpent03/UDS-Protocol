#pragma once

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

typedef struct ISO_TP_Frame {
  /* 11-bit CAN-ID */
  uInt16 addr;
  uInt8 data[8]; 
} ISO_TP_Frame;

enum ISO_TP_CODES {
  DEFAULT_RX_ADDR = 0x731,
  DEFAULT_TX_ADDR = 0x739,
  DEFAULT_DLC = 0x8
};

/** 
 * @todo IN_BUF triggers a flag to start receive stream
 * @todo create a server loop mechanism for ECU states
 */

// /**
//  * @brief Deallocates the allocated CAN-TP frame.
//  * @param cfr The CAN-TP frame to be deallocated.
//  * @return void.
//  * @META This is deprecated now.
//  */
// void dealloc_CANTP_frame(CAN_Frame* cfr);

/**
 * @brief Takes the data_queue and the dataLength, and returns a single ISO-TP frame.
 * @param data_queue the data_queue filled with the UDS packet data.
 * @param dataLength the length of the data_queue.
 * @param ITFR The ISO-TP frame to perform the operation on.
 * @return void.
 */
void CANTP_single_frame(ISO_TP_Frame *ITFR, queue* data_queue, uInt16 dataLength);

/**
 * @brief Generates the first ISO-TP frame for segmented UDS data.
 * @param data_queue The queue filled with UDS packet data.
 * @param dataLength Length of the total UDS packet data.
 * @param ITFR The ISO-TP frame to perform the operation on.
 * @return void.
 */ 
void CANTP_first_frame(ISO_TP_Frame *ITFR, queue* data_queue, uInt16 dataLength);

/**
 * @brief Generates a consecutive ISO-TP frame after the ISO-TP first frame.
 * @param data_queue The queue filled with UDS packet data.
 * @param sequenceNum The sequence number for the currently transmitted frame.
 * @param ITFR The ISO-TP frame to perform the operation on.
 * @return void.
 */
void CANTP_consec_frame(ISO_TP_Frame *ITFR, queue* data_queue, uInt8 sequenceNum);

void CANTP_flow_control_frame(ISO_TP_Frame *cfr);

/**
 * @brief This generates the CAN_ID + ISO-TP frames for the given UDS packet data.
 * @param udsp the required UDS packet.
 * @param rx_addr The address to send data to.
 * @return void.
 */
void send_ISOTP_frames(UDS_Packet *udsp, uInt16 rx_addr);

/**
 * @brief Take the incoming byte stream from the GPIO and convert it to a UDS packet.
 * @param udsp The UDS packet object for the data to be written to.
 * @return True if the operation was successful.
 */
 bool receive_ISOTP_frames(UDS_Packet *udsp);

/**
 * @brief Initiates the routine for sending the ISO_TP frame(address + CAN_TP frame) to GPIO
 * @param cfr The CAN_TP frame to send.
 * @return void.
 */
void populate_output_buffer(ISO_TP_Frame *ITFR);

