#include "common.h"
#include <stdbool.h>

typedef struct queue { 
  uInt8 *arr;
  Int16 front;
  Int16 rear;
  uInt16 size;
} queue;

bool queue_isfull(queue *q);
bool queue_isempty(queue *q);
queue* init_queue(uInt16 size);
bool enque(queue *q, uInt8 data);
bool dequeue(queue *q);
bool at_queue_front(queue *q, uInt8 *data);
bool at_queue_rear(queue *q, uInt8 *data);
void free_queue(queue *q);
