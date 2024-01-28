#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef unsigned char uInt8;
typedef unsigned short int uInt16;
typedef unsigned int uInt32;
typedef char Int8;
typedef short int Int16;
typedef int Int32;

/**
* @todo CAN_Packet -> [1, 3] byte PCI, rest is data.
* @todo define a ISO_TP frame enveloping the CAN_packet with the relevant CAN_ID
*/
