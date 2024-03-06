#include "../common.h"
#include "../UTILS/utils.h"
#include "test.h"
#include "../BUS/bus.h"

bool DEBUG;

void set_debug_bool(bool boolean) {
  DEBUG = boolean;
}
bool get_debug_bool() {
  return DEBUG;
}

int main(int argc, char **argv) {
  if (argc < 11) {
    printf("Please input all the ten required bytes!\n");
    return -1;
  }
  uInt8 TEST_PACKET[10];
  for (int i = 1; i < 11; i++) {
    TEST_PACKET[i] = stoh(argv[i]);
  }
  for (int i = 0; i < 10; i++) {
    printf("%02x ", TEST_PACKET[i]);
  }
  return 0;
}
