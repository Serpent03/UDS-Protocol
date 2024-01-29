#include "common.h"
#include "ISOTP_CAN.h"
#include "queue.h"

int main() {
  uInt8 data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 0x69};
  UDS_Packet *tx = generate_UDS_packet(SID_ECU_RESET, data, sizeof(data) / sizeof(uInt8));
  send_ISOTP_frames(tx);

  UDS_Packet rx;
  bool opSuccess = receive_ISOTP_frames(&rx);
  if (opSuccess) {
    printf("\nSID: 0x%02X\n", rx.SID);
    for (uInt8 i = 0; i < rx.dataLength; i++) {
      printf("DAT: 0x%02X\n", rx.data[i]);
    }
  } else {
    printf("Opfail\n");
  }
  /* Call the "parse" function here for the incoming UDS packet */
}
