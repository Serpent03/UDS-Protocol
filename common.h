#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

typedef uint8_t uInt8;
typedef uint16_t uInt16;
typedef uint32_t uInt32;
typedef uint64_t uInt64;

typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef float Float32;
typedef double Float64;

#define TX_RETRY_LIMIT 3
#define TX_TIME_LIMIT 150 /* The transmitting message should happen in 150ms */

extern uInt64 CLOCK_TIME_AT_TX;
extern uInt64 CLOCK_TIME_AT_RX;

/**
 * @brief Gets the current UNIX timestamp.
 * @returns Current UNIX timestamp.
 */
uInt64 getTime();

/**
 * @brief Sets the current UNIX timestamp inside the passed variable.
 * @param VAR The 32-bit unsigned integer to put the timestamp in.
 * @returns void.
 */
void setTime(uInt64 *VAR);
