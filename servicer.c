#include "common.h"
#include "ISOTP_CAN.h"
#include "servicer.h"
#include "UDS.h"
#include <sys/time.h>

bool receiveFlag = false;
bool transmitFlag = true;
bool idle = true;
bool shutdown = false;

struct timeval tp;

uInt64 CLOCK_TIME_START;
uInt64 CLOCK_TIME_CURRENT;
uInt64 CLOCK_TIME_OLD;

uInt64 CLOCK_TIME_AT_TX;
uInt64 CLOCK_TIME_AT_RX;


uInt64 getTime() {
  gettimeofday(&tp, NULL);
  uInt64 ret_val = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
  return ret_val;
}

void setTime(uInt64 *VAR) {
  gettimeofday(&tp, NULL);
  *VAR = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

void servicer() {
  if (receiveFlag && idle) {
    idle = false;
    receiveFlag = false;
    /* SYNC THE RX TIME HERE */
    /** @todo Implement receiver timeout. */
    setTime(&CLOCK_TIME_AT_RX);

    bool opSuccess = receive_ISOTP_frames(&uds_rx);
    if (opSuccess) {
      printf("\nSID: 0x%02X\n", uds_rx.SID);
      for (uInt16 i = 0; i < uds_rx.dataLength; i++) {
        printf("DAT: 0x%02X\n", uds_rx.data[i]);
      }
    } else {
      printf("Opfail: GPIO does not exist or it is empty.\n");
    }
  }

  /* Here we will call the parse() function which decides on the transmit flag. */
  uInt8 data[8] = { 0 };
  UDS_Packet *tx = generate_UDS_packet(SID_ECU_RESET, data, sizeof(data) / sizeof(uInt8));
  /* The data for transmit is decided by the parse() function, so we'll define the data here. */
  
  if (idle && transmitFlag) {
    transmitFlag = false;
    idle = false; 
    uInt8 TX_RETRY_NUM = 0;

    /* Sync the tx time here. We'll compare time elapsed against CLOCK_TIME_TX for aborting due to timeout. */
    setTime(&CLOCK_TIME_AT_TX);
    while (!send_ISOTP_frames(tx, DEFAULT_RX_ADDR) && TX_RETRY_NUM < TX_RETRY_LIMIT) {
      TX_RETRY_NUM++;
      printf("TRY %d FOR TX!\n", TX_RETRY_NUM);
      /**
       * @todo Verify try limit for transmission matches reality.
       * If the transmitter has exhausted all successful attempts, it will go back to default 
       * and will not expect any transmissions coming back.
       * I don't know if this is realistic. */
    }
    receiveFlag = TX_RETRY_NUM < TX_RETRY_LIMIT; /* Set it to true for debug here. */
  }
  idle = true;
  printf("CURTIME: %lu\n", CLOCK_TIME_CURRENT);
}

void Server_Init() {
  setTime(&CLOCK_TIME_START);
  setTime(&CLOCK_TIME_CURRENT);
  CLOCK_TIME_OLD = CLOCK_TIME_CURRENT;

  Server_Main();
}

void Server_Main() {
  while (!shutdown) {
    gettimeofday(&tp, NULL);
    CLOCK_TIME_CURRENT = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);

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
