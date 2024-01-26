#include "common.h"

int main() {
  UDS_Packet *udsp_req = generate_UDS_packet(REQUEST, 0x10, 0x00, 0xAA, 0xAA);
  // a function to parse the UDS packet.
}
