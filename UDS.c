#include "common.h"

UDS_Packet* generate_UDS_packet(enum UDS_PACKET_TYPE packetType, unsigned char param1, unsigned char param2, unsigned short payload_1, unsigned short payload_2) {
  UDS_Packet *udsp = (UDS_Packet*)calloc(1, sizeof(UDS_Packet));
  udsp->param_1 = param1;
  udsp->param_2 = param2;
  udsp->payload_1 = payload_1;
  if (packetType != NEGRESP) {
    udsp->payload_2 = payload_2;
  }
  return udsp;
}

UDS_Packet* parse(UDS_Packet *udsp) {
  UDS_Packet *returnPacket;
  // process the packet and send back either a positive response or a negative one
  return returnPacket;
}
