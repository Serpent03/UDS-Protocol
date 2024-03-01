#pragma once

#include "../common.h"
#include "../UDS/UDS.h"
#include "../QUEUE/queue.h"

#define ISOTP_PADDING 0xAA
#define TX_RETRY_LIMIT 3

enum CAN_FRAME_CODES {
  CAN_CODE_SINGLE_FRAME = 0x0,
  CAN_CODE_FIRST_FRAME = 0x1,
  CAN_CODE_CONSEC_FRAME = 0x2,
  CAN_CODE_FLOW_CNTL_FRAME = 0x3 ,
  CAN_FC_FLAG_0 = 0x0,
  CAN_FC_FLAG_1 = 0x1,
  CAN_FC_FLAG_2 = 0x2,
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

enum ISOTP_CODES {
  DEFAULT_SERVER_ADDR = 0x731,
  DEFAULT_CLIENT_ADDR = 0x739,
  DEFAULT_DLC = 0x8
};

/**
 * @brief Takes the data_queue and the dataLength, and returns a single ISO-TP frame.
 * @param data_queue the data_queue filled with the UDS packet data.
 * @param dataLength the length of the data_queue.
 * @param ITFR The ISO-TP frame to perform the operation on.
 * @return True if the operation is successful.
 */
bool CANTP_single_frame(ISO_TP_Frame *ITFR, queue* data_queue, uInt16 dataLength);

/**
 * @brief Generates the first ISO-TP frame for segmented UDS data.
 * @param data_queue The queue filled with UDS packet data.
 * @param dataLength Length of the total UDS packet data.
 * @param ITFR The ISO-TP frame to perform the operation on.
 * @return True if the operation was successful.
 */ 
bool CANTP_first_frame(ISO_TP_Frame *ITFR, queue* data_queue, uInt16 dataLength);

/**
 * @brief Generates a consecutive ISO-TP frame after the ISO-TP first frame.
 * @param data_queue The queue filled with UDS packet data.
 * @param sequenceNum The sequence number for the currently transmitted frame.
 * @param ITFR The ISO-TP frame to perform the operation on.
 * @return True if the operation was successful.
 */
bool CANTP_consec_frame(ISO_TP_Frame *ITFR, queue* data_queue, uInt16 sequenceNum);


/**
 * @brief Sets the flow control mechanism variables(BS, STmin) for outgoing transmissions.
 * @return True if successful.
 * @todo remove dependency on FILE I/O.
 */
bool CANTP_read_flow_control_frame();

/**
 * @brief Generates a flow control frame for the sender, from the receiver side.
 * @param addr The address to write the flow control frame to.
 * @return True if successful.
 * @todo remove dependency on FILE I/O.
 */
bool CANTP_write_flow_control_frame(uInt16 addr);

/**
 * @brief This generates the CAN_ID + ISO-TP frames for the given UDS packet data.
 * @param udsp the required UDS packet.
 * @param from_addr The returning arbitration ID(usually receiving ID + 0x8) 
 * @return True if the operation was successful.
 * @FIX Effective limit is 4094 instead of 4096.
 */
bool send_ISOTP_frames(UDS_Packet *udsp, uInt16 from_addr);

/**
 * @brief Take the incoming byte stream from the GPIO and convert it to a UDS packet.
 * @param udsp The UDS packet object for the data to be written to.
 * @return True if the operation was successful.
 * @FIX Effective limit is 4094 instead of 4096.
 */
 bool receive_ISOTP_frames(UDS_Packet *udsp);

/**
 * @brief Initiates the routine for sending the ISO_TP frame(address + CAN_TP frame) to GPIO
 * @param cfr The CAN_TP frame to send.
 * @return void.
 */
void populate_output_buffer(ISO_TP_Frame *ITFR);

/**
 * @brief Call the send_ISOTP_frames() function with an error handling wrapper.
 * @param udsp The UDS packet to send.
 * @param silenceTx If true, does not transmit at all.
 * @param addr The arbitration/address ID of the ISO-TP frame.
 */
void send_UDSonCAN(UDS_Packet *udsp, bool silenceTx, uInt16 addr);

