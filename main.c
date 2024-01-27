#include "common.h"

int main() {
  UDS_Packet *tx = generate_UDS_packet(SID_DIAGNOSTIC_SESS_CNTL, NULL, 0);
  UDS_Packet *rx = parse(tx);
  printf("0x%02x, %d, 0x%02x\n", rx->SID, rx->dataLength, rx->data[0]);
}
