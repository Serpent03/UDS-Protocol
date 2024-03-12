#pragma once

#include "../common.h"

typedef struct SESSION {
  uInt16 RANGE_START;
  uInt16 RANGE_END;
  uInt16 TX_ADDR;
  uInt16 REPLY_ADDR;
} SESSION;

/**
 * @brief Set the CAN-ID session range.
 * @param start Start of the 11-bit range.
 * @param end End of the 11-bit range.
 * @return void.
 * @todo remove dependency on FILE I/O.
 */
void set_session(uInt16 start, uInt16 end, uInt16 tx, bool isTx);

/**
 * @brief Gets the transmit address/arbitration ID for the current session.
 * @return An 11-bit CAN arbitration ID.
 */
uInt16 get_tx_addr();

/**
 * @brief Gets the reply address/arbitration ID for the current session.
 * @return An 11-bit CAN arbitration ID.
 */
uInt16 get_reply_addr();

/**
 * @brief Sets the reply address/arbitration ID for the current session.
 * @param addr The replying address(which is usually receive address + 0x8).
 * @return void.
 */
void set_reply_addr(uInt16 addr);

/**
 * @brief Reset the bus after a failed transmission.
 * @return void.
 */
void reset_bus();

/**
 * @brief Monitor the bus for any activity.
 * @return void.
 * @todo remove dependency on FILE I/O.
 */
void check_bus();

/**
 * @brief Write data to the CAN bus.
 * @param OUT_BUF The output buffer to write to the bus.
 * @param size Size of the output buffer.
 * @return void.
 * @todo remove dependency on FILE I/O.
 */
void write_to_bus(uInt8 *OUT_BUF, size_t size);

/**
 * @brief Read data from the CAN bus.
 * @param IN_BUF The input buffer to populate.
 * @param size Size of the input buffer.
 * @return True if operation was successful.
 * @todo remove dependency on FILE I/O.
 */
bool read_from_bus(uInt8 *IN_BUF, size_t size);

/**
 * @brief Fetch the new bus data from the CAN bus.
 * @param IN_BUF The input buffer to populate.
 * @param size Size of the input buffer.
 * @return True if it found new data on the bus.
 * @todo remove dependency on FILE I/O.
 */
bool new_bus_data(uInt8 *IN_BUF, size_t size);
