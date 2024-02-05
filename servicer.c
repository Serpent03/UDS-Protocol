#include "common.h"
#include "ISOTP_CAN.h"
#include "servicer.h"
#include "UDS.h"
#include <sys/time.h>

bool receiveFlag = false;
bool transmitFlag = true;
bool idle = true;
struct timeval tp;
uInt32 CLOCK_TIME_START;
uInt32 CLOCK_TIME_CURRENT;
uInt32 CLOCK_TIME_OLD;

void servicer() {
  if (receiveFlag && idle) {
    idle = false;
    receiveFlag = false;
    bool opSuccess = receive_ISOTP_frames(&uds_rx);
    if (opSuccess) {
      printf("\nSID: 0x%02X\n", uds_rx.SID);
      for (uInt16 i = 0; i < uds_rx.dataLength; i++) {
        printf("DAT: 0x%02X\n", uds_rx.data[i]);
      }
    } else {
      printf("Opfail: GPIO does not exist or it is empty.\n");
    }
    // exit(0);
  }

  /* Here we will call the parse() function which decides on the transmit flag. */
  
  if (idle && transmitFlag) {
    transmitFlag = false;
    idle = false; 

    uInt8 data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0x42 };
    UDS_Packet *tx = generate_UDS_packet(SID_ECU_RESET, data, sizeof(data) / sizeof(uInt8));
    send_ISOTP_frames(tx, DEFAULT_RX_ADDR);
    receiveFlag = true; /* Set it to true for debug here. */
  }
  idle = true;
  printf("CURTIME: %u\n", CLOCK_TIME_CURRENT);
}

void Server_Init() {
  gettimeofday(&tp, NULL);
  CLOCK_TIME_START = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
  gettimeofday(&tp, NULL);
  CLOCK_TIME_CURRENT = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
  CLOCK_TIME_OLD = CLOCK_TIME_CURRENT;
  Server_Main();
}

void Server_Main() {
  while (true) {
    gettimeofday(&tp, NULL);
    CLOCK_TIME_CURRENT = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);

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
