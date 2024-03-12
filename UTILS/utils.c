#include "../common.h"
#include "utils.h"

uInt16 mapToInt(char c) {
  uInt16 _int;
  if (c >= '0' && c <= '9') {
    _int = c - '0';
  } else if (c >= 'a' && c <= 'f') {
    _int = c - 'a' + 10;
  } else if (c >= 'A' && c <= 'F') {
    _int = c - 'A' + 10;
  } else {
    printf("Not a valid hexadecimal character!\n");
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

void insertIntoArray(uInt8 *array, uInt8 _data, uInt16 *idx) {
  /* We use this to ensure as least as possible errors during insertion into array using pointers */
  array[(*idx)++] = _data;
}

void insertInArraytoByte(uInt8 *array, uInt16 *idx, void *_data, size_t size) {
  for (uInt16 i = size - 1; i >= 0; i--) {
    /** @todo complete this function. */
    // insertIntoArray(array, , idx);
  }
}
