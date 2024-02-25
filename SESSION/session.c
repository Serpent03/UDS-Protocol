#include "../common.h"
#include "../ISOTP/ISOTP_CAN.h"
#include "session.h"
#include "../UDS/UDS.h"
#include "../BUS/bus.h"
#include "../SERVICES/services.h"
#include "timing.h"

bool receiveFlag;
bool transmitFlag;
bool isTransmitter;
bool processFlag;
bool idle;
bool shutdown;

uInt64 CLOCK_TIME_START;
uInt64 CLOCK_TIME_CURRENT;
uInt64 CLOCK_TIME_OLD;

uInt64 CLOCK_TIME_AT_TX;
uInt64 CLOCK_TIME_AT_RX;

UDS_Packet *tx;
bool silenceTransmit;

void servicer() {
  if (receiveFlag && idle) {
    idle = false;
    receiveFlag = false;

    bool opSuccess = receive_ISOTP_frames(&uds_rx);
    if (opSuccess) {
      printf("\nSID: 0x%02X\n", uds_rx.SID);
      for (uInt16 i = 0; i < uds_rx.dataLength; i++) {
        // printf("DAT: 0x%02X\n", uds_rx.data[i]);
        assert(uds_rx.data[i] == i);
      }
      processFlag = true; /* Start operation on the data */
    } else {
      printf("Opfail: GPIO does not exist or it is empty.\n");
    }
  }

  /* Here we will call the parse() function which decides on the transmit flag. */
  uInt8 data[13];
  for (Int16 i = 0; i < 13; i++) {
    data[i] = i;
  }
  tx = generate_UDS_packet(SID_DIAGNOSTIC_SESS_CNTL, data, sizeof(data) / sizeof(uInt8));
  if (idle && processFlag) {
    tx = service_handler(&uds_rx, &silenceTransmit);
    processFlag = false;
    shutdown = true; /** @debug */

    send_UDSonCAN(tx, silenceTransmit, get_reply_addr());  
  }

  if (idle && transmitFlag) {
    transmitFlag = false;
    idle = false; 

    send_UDSonCAN(tx, false, DEFAULT_SERVER_ADDR);
    // receiveFlag = TX_RETRY_NUM < TX_RETRY_LIMIT; /* Set it to true for debug here. */
  }

  if (idle) {
    check_bus();
    transmitFlag = !processFlag && transmitFlag; /** @debug For testing reply-back. */
    // isTransmitter = false;
  }
  idle = true;
  // printf("CURTIME: %lu\n", CLOCK_TIME_CURRENT);
}

void Server_Init() {
  setTime(&CLOCK_TIME_START);
  setTime(&CLOCK_TIME_CURRENT);
  CLOCK_TIME_OLD = CLOCK_TIME_CURRENT;
  receiveFlag = false;
  transmitFlag = isTransmitter;
  silenceTransmit = false;
  // isTransmitter = transmitFlag;
  processFlag = false;
  idle = true;
  shutdown = false;
}

void Server_Main() {
  while (!shutdown) {
    setTime(&CLOCK_TIME_CURRENT);

    /* This prevents the function from firing off in the same second multiple times. */
    if (CLOCK_TIME_CURRENT != CLOCK_TIME_OLD) {
      CLOCK_TIME_OLD = CLOCK_TIME_CURRENT;

      switch (CLOCK_TIME_CURRENT % CLOCK_CYCLE) {
        case 0:
          servicer();
          continue;
          break;
        default:
          break;
      }
    }
  }
}
