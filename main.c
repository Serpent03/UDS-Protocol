#include "BUS/bus.h"
#include "common.h"
#include "./SERVICER/servicer.h"

int main(int argc, char **argv) {
  /** @todo String to hexadecimal converter. */
  set_session(atoi(argv[1]), atoi(argv[2]));
  Server_Init();
  Server_Main();
}
