#pragma once

#include "../common.h"

typedef struct SESSION {
  uInt16 RANGE_START;
  uInt16 RANGE_END;
} SESSION;

/**
 * @brief Set the CAN-ID session range.
 * @param start Start of the 11-bit range.
 * @param end End of the 11-bit range.
 * @returns void.
 * @todo remove dependency on FILE I/O.
 */
void set_session(uInt16 start, uInt16 end, bool isTx);

/**
 * @brief Monitor the bus for any activity.
 * @returns void.
 * @todo remove dependency on FILE I/O.
 */
void check_bus();

/**
 * @brief Write data to the CAN bus.
 * @param OUT_BUF The output buffer to write to the bus.
 * @param size Size of the output buffer.
 * @returns void.
 * @todo remove dependency on FILE I/O.
 */
void write_to_bus(uInt8 *OUT_BUF, size_t size);

/**
 * @brief Read data from the CAN bus.
 * @param IN_BUF The input buffer to populate.
 * @param size Size of the input buffer.
 * @returns True if operation was successful.
 * @todo remove dependency on FILE I/O.
 */
bool read_from_bus(uInt8 *IN_BUF, size_t size);

bool new_bus_data(uInt8 *IN_BUF, size_t size);
