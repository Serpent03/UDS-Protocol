#include "../common.h"
#include "utils.h"

uInt16 mapToInt(char c) {
  uInt16 _int;
  if (c >= '0' && c <= '9') {
    _int = c - '0';
  } else if (c >= 'a' && c <= 'f') {
    _int = c - 'a';
  } else if (c >= 'A' && c <= 'F') {
    _int = c - 'A';
  } else {
    assert(false); /* Raise error and abort the program before it begins. */
  }
  return _int;
}

uInt16 stoh(char* string) {
  uInt16 result = 0;
  char c;

  if (string[0] == '0' && (string[1] == 'x' || string[1] == 'X')) {
    string = &string[2]; /* Skip the leading 0x or 0X */
  }

  while ((c = *string++) != '\0') {
    result *= 16;
    result += mapToInt(c);
  }
  return result;
}
