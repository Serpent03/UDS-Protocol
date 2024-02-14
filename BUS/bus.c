#include "bus.h"
#include "../SERVICER/servicer.h"

FILE *bus;
uInt8 NUL_BUF[10] = { 0 };
uInt8 BUS_DATA[10];
SESSION can_session;

void set_session(uInt16 start, uInt16 end, bool isTx) {
  can_session.RANGE_START = start;
  can_session.RANGE_END = end;
  isTransmitter = isTx;
  /** @debug isTx is set to control tx/rx nodes for now. */
  printf("SESSION SET @ 0x%02x :: 0x%02x\n", start, end);
}

void check_bus() {
  bus = fopen("bus.bin", "rb");
  if (bus == NULL) {
    return;
  }
  fread(BUS_DATA, 10, sizeof(uInt8), bus);
  if (memcmp(BUS_DATA, NUL_BUF, 10) == 0) {
    return;
  }
  /** @todo Flesh the address filtration. */
  uInt16 CAN_ID = ((BUS_DATA[0] << 8) | BUS_DATA[1]) >> 5;
  if (CAN_ID < can_session.RANGE_START || CAN_ID > can_session.RANGE_END) {
    return;
  }
  receiveFlag = true;
  /* At this point, the servicer() takes over. */
}

void write_to_bus(uInt8 *OUT_BUF, size_t size) {
  bus = fopen("bus.bin", "wb");
  fwrite(OUT_BUF, sizeof(uInt8), size, bus);
  fclose(bus);
}

bool read_from_bus(uInt8 *IN_BUF, size_t size) {
  bus = fopen("bus.bin", "rb");
  if (bus == NULL) {
    return false;
  }
  fread(IN_BUF, size, sizeof(uInt8), bus);
  fclose(bus);
  return true;
}

