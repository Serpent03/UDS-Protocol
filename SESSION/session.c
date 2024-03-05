#include "../common.h"
#include "../ISOTP/ISOTP_CAN.h"
#include "session.h"
#include "state.h"
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
      for (int i = 0; i < uds_rx.dataLength; i++) {
        printf("DAT: 0x%02X\n", uds_rx.data[i]);
      }
      processFlag = true; /* Start operation on the data */
    } else {
      printf("Opfail: GPIO does not exist or it is empty.\n");
    }
  }

  uInt8 data[] = { 0x1 };
  tx = generate_UDS_packet(SID_DIAGNOSTIC_SESS_CNTL, data, sizeof(data) / sizeof(uInt8));
  // if (get_debug_bool()) {
  //   tx = generate_UDS_packet(SID_STATE_DEBUG, data, 0);
  // }

  if (idle && processFlag) {
    tx = service_handler(&uds_rx, &silenceTransmit);
    processFlag = false;
    shutdown = true; /** @debug */

    /* The nodes that are only transmitters do not reply, for now. */
    send_UDSonCAN(tx, isTransmitter, get_reply_addr());  
  }

  if (idle && transmitFlag) {
    transmitFlag = false;
    idle = false; 

    send_UDSonCAN(tx, false, get_tx_addr());
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
  set_state(STATE_DIAGNOSTIC_SESSION, 0x1); /* by default the program starts in the default diagnostic session */
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

      if (!!CLOCK_TIME_CURRENT % CLOCK_CYCLE) {
        servicer();
      }
      update_state();
    }
  }
}
