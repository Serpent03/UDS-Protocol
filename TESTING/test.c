#include "../common.h"
#include "test.h"

bool DEBUG;

void set_debug_bool(bool boolean) {
  DEBUG = boolean;
}
bool get_debug_bool() {
  return DEBUG;
}
