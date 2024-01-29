#include "common.h"
#include "ISOTP_CAN.h"
#include "queue.h"

int main() {
  uInt8 data[] = {1, 2, 3, 4};
  UDS_Packet *tx = generate_UDS_packet(SID_READ_DATA_BY_IDENT, data, sizeof(data) / sizeof(uInt8));
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
