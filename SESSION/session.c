#include "../common.h"
#include "../ISOTP/ISOTP_CAN.h"
#include "session.h"
#include "../UDS/UDS.h"
#include "../BUS/bus.h"
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
  UDS_Packet *tx = generate_UDS_packet(SID_DIAGNOSTIC_SESS_CNTL, data, sizeof(data) / sizeof(uInt8));
  if (idle && processFlag) {
    tx = parse(&uds_rx);
    processFlag = false;
    shutdown = true; /** @debug */

    uInt8 TX_RETRY_NUM = 1;
    while (!send_ISOTP_frames(tx, DEFAULT_SERVER_ADDR + 0x8) && TX_RETRY_NUM < TX_RETRY_LIMIT) {
      printf("TRY %d FOR TX!\n\n", TX_RETRY_NUM);
      TX_RETRY_NUM++;
    }
  }
  /* The data for transmit is decided by the parse() function, so we'll define the data here. */
  
  if (idle && transmitFlag) {
    transmitFlag = false;
    idle = false; 
    uInt8 TX_RETRY_NUM = 1;

    /* Sync the tx time here. We'll compare time elapsed against CLOCK_TIME_TX for aborting due to timeout. */
    while (!send_ISOTP_frames(tx, DEFAULT_SERVER_ADDR) && TX_RETRY_NUM < TX_RETRY_LIMIT) {
      printf("TRY %d FOR TX!\n\n", TX_RETRY_NUM);
      TX_RETRY_NUM++;
    }
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
