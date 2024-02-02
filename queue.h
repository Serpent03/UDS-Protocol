#pragma once

#include "common.h"

typedef struct queue { 
  uInt8 arr[4096];
  Int16 front;
  Int16 rear;
  uInt16 size;
} queue;

/**
 * @brief Returns a boolean on whether the queue is full.
 * @param q The queue.
 * @return True if the queue is full.
 */
bool queue_isfull(queue *q);

/**
 * @brief Returns a boolean on whether the queue is empty.
 * @param q The queue.
 * @return True if the queue is empty.
 */
bool queue_isempty(queue *q);

/**
 * @brief Returns an initialized queue.
 * @param size The size of the new queue.
 * @return A pointer to the queue structure.
 */
queue* init_queue(uInt16 size);

/**
 * @brief Adds an element to the rear of the queue.
 * @param q The queue to operate on.
 * @param data The 8-bit unsigned data to add
 * @return True if the operation was successful.
 */
bool enque(queue *q, uInt8 data);

/**
 * @brief Removes an element from the front of the queue.
 * @param q The queue to operate on.
 * @return True if the operation was successful.
 */
bool dequeue(queue *q);

/**
 * @brief Returns an element at the front of the queue.
 * @param q The queue to operate on.
 * @param data Pointer to an uInt8 variable to store the value.
 * @return True if the operation was successful in fetching a value from the queue.
 */
bool at_queue_front(queue *q, uInt8 *data);

/**
 * @brief Returns an element at the rear of the queue.
 * @param q The queue to operate on.
 * @param data Pointer to an uInt8 variable to store the value.
 * @return True if the operation was successful in fetching a value from the queue.
 */
bool at_queue_rear(queue *q, uInt8 *data);

// /**
//  * @brief Frees the queue data structure from the program memory.
//  * @param q The queue to operate on.
//  * @return void.
//  * @META Deprecated now.
//  */
// void free_queue(queue *q);

/**
 * @brief Returns a 16-bit unsigned integer containing the length of the queue.
 * @param q The queue to operate on.
 * @return uInt16 variable containing the length of the queue.
 */
uInt16 len_queue(queue *q);


