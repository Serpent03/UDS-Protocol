#include "common.h"
#include "queue.h"
#include "UDS.h"

int main() {
  UDS_Packet *tx = generate_UDS_packet(SID_ECU_RESET, NULL, 0);
}
