#include "common.h"
#include "queue.h"
#include "CAN.h"

int main() {
  uInt8 data[] = {0x01, 0x02, 0x69};
  UDS_Packet *tx = generate_UDS_packet(SID_DIAGNOSTIC_SESS_CNTL, data, 3);
  printf("data: 0x%02x\n", tx->data[0]);
  printf("data: 0x%02x\n", tx->data[1]);
  printf("data: 0x%02x\n", tx->data[2]);
  generate_ISOTP_frames(tx);
}
