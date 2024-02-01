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
  uInt8 data[8];
} CAN_Frame;

typedef struct ISO_TP_Frame {
  uInt16 addr;
  CAN_Frame *cfr; 
} ISO_TP_Frame;

enum defaultAddress {
  DEFAULT_RX_ADDR = 0x6789,
  DEFAULT_TX_ADDR = 0x1234,
};

/** 
 * @todo IN_BUF triggers a flag to start receive stream
 * @todo define a ISO_TP frame enveloping the CAN_packet with the relevant CAN_ID
 */

/**
 * @brief Deallocates the allocated CAN-TP frame.
 * @param cfr The CAN-TP frame to be deallocated.
 * @return void.
 * @META This is deprecated now.
 */
void dealloc_CANTP_frame(CAN_Frame* cfr);

/**
 * @brief Takes the data_queue and the dataLength, and returns a single CAN-TP frame.
 * @param data_queue the data_queue filled with the UDS packet data.
 * @param dataLength the length of the data_queue.
 * @param cfr The CAN-TP frame to perform the operation on.
 * @return void.
 */
void CANTP_single_frame(CAN_Frame *cfr, queue* data_queue, uInt16 dataLength);

/**
 * @brief Generates the first CAN-TP frame for segmented UDS data.
 * @param data_queue The queue filled with UDS packet data.
 * @param dataLength Length of the total UDS packet data.
 * @param cfr The CAN-TP frame to perform the operation on.
 * @return void.
 */ 
void CANTP_first_frame(CAN_Frame *cfr, queue* data_queue, uInt16 dataLength);

/**
 * @brief Generates a consecutive CAN-TP frame after the CAN-TP first frame.
 * @param data_queue The queue filled with UDS packet data.
 * @param sequenceNum The sequence number for the currently transmitted frame.
 * @param cfr The CAN-TP frame to perform the operation on.
 * @return void.
 */
void CANTP_consec_frame(CAN_Frame *cfr, queue* data_queue, uInt8 sequenceNum);

/**
 * @brief This generates the CAN_ID + CAN_frames for the given UDS packet data.
 * @param udsp the required UDS packet.
 * @todo use the entire ISO_TP header for proper addressing
 * @return void.
 */
void send_ISOTP_frames(UDS_Packet *udsp);

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
void populate_output_buffer(CAN_Frame* cfr);

