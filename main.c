#include "common.h"
#include "ISOTP_CAN.h"
#include "queue.h"

int main() {
  uInt8 data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  UDS_Packet *tx = generate_UDS_packet(SID_ECU_RESET, data, sizeof(data) / sizeof(uInt8));
  send_ISOTP_frames(tx, DEFAULT_RX_ADDR);

  UDS_Packet rx;
  bool opSuccess = receive_ISOTP_frames(&rx);

  if (opSuccess) {
    printf("\nSID: 0x%02X\n", rx.SID);
    for (uInt8 i = 0; i < rx.dataLength; i++) {
      printf("DAT: 0x%02X\n", rx.data[i]);
    }
  } else {
    printf("Opfail: GPIO does not exist or it is empty.\n");
  }
  /* Call the "parse" function here for the incoming UDS packet */
}
