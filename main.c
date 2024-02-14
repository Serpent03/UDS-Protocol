#include "BUS/bus.h"
#include "common.h"
#include "./SERVICER/servicer.h"
#include "UTILS/utils.h"

int main(int argc, char **argv) {
  set_session(stoh(argv[1]), stoh(argv[2]));
  Server_Init();
  Server_Main();
}
