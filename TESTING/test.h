#pragma once 

#include "../common.h"
#include "../UDS/UDS.h"

/**
 * @brief Get the testing variable to true.
 * @return the state of the testing variable.
 */
bool get_debug_bool();

/**
 * @brief Set the testing variable to true.
 * @param boolean The boolean to set.
 * @return void.
 */
void set_debug_bool(bool boolean);


