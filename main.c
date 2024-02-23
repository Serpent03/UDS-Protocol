#include "BUS/bus.h"
#include "common.h"
#include "./SESSION/session.h"
#include "UTILS/utils.h"

int main(int argc, char **argv) {
  set_session(stoh(argv[1]), stoh(argv[2]), strcmp(argv[3], "1") == 0);
  Server_Init();
  Server_Main();
}
