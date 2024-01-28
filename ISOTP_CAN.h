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

/** @todo utilize input and output buffer */

/**
 * @brief Takes the data_queue and the dataLength, and returns a single CAN_TP frame.
 * @param data_queue the data_queue filled with the UDS packet data.
 * @param dataLength the length of the data_queue.
 * @return A CAN frame object.
 */
CAN_Frame* single_CANTP_frame(queue* data_queue, uInt16 dataLength);

/**
 * @brief Takes the data_queue and starts a subroutine for multiple CAN_TP frames.
 * @param data_queue the data_queue filled with the UDS packet data.
 * @return A CAN frame object.
 */
CAN_Frame* multiple_CANTP_frame(queue* data_queue);

/**
 * @brief This generates the CAN_ID + CAN_frames for the given UDS packet data.
 * @param udsp the required UDS packet.
 * @todo use the entire ISO_TP header for proper addressing
 * @return void.
 */
void send_ISOTP_frames(UDS_Packet *udsp);

/**
 * @brief Initiates the routine for sending the ISO_TP frame(address + CAN_TP frame) to GPIO
 * @param cfr The CAN_TP frame to send.
 * @return void.
 */
void populate_output_buffer(CAN_Frame* cfr);

