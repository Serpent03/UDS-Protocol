#include "BUS/bus.h"
#include "common.h"
#include "./SESSION/session.h"
#include "UTILS/utils.h"
#include "TESTING/test.h"

int main(int argc, char **argv) {
  set_session(stoh(argv[1]), stoh(argv[2]), stoh(argv[3]), strcmp(argv[4], "1") == 0);
  Server_Init();
  Server_Main();

  // 
}
