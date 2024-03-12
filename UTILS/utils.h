#pragma once 

#include "../common.h"

/**
 * @brief Convert a string into a 16-bit hexadecimal value.
 * @param string The input string.
 * @return A 16-bit unsigned integer.
 */
uInt16 stoh(char* string);

/**
 * @brief Insert an element into the array.
 * @param array The pointer to the array.
 * @param _data The data to be inserted.
 * @param idx The address of the length of the data.
 * @return void.
 */
void insertIntoArray(uInt8 *array, uInt8 _data, uInt16 *idx);

/**
 * @brief Insert an element larger than 1-byte into an array of 1-byte numbers.
 * @param array The pointer to the array.
 * @param _data The pointer to the data to be inserted.
 * @param idx The pointer to the length of the data.
 * @param size Size in bytes of the data.
 * @return void.
 */
void insertInArraytoByte(uInt8 *array, uInt16 *idx, void *_data, size_t size);

