#include "common.h"
#include "ISOTP_CAN.h"
#include "queue.h"

int main() {
  uInt8 data[] = {0x01, 0x02, 0x69};
  UDS_Packet *tx = generate_UDS_packet(SID_ECU_RESET, data, 3);
  send_ISOTP_frames(tx);
}
