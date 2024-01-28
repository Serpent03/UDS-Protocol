#include "queue.h"

bool queue_isfull(queue *q) {
  return (
    (q->front == 0 && q->rear == q->size - 1)
    ||
    (q->front - 1 == q->rear)
  );
}
bool queue_isempty(queue *q) {
  return q->front == -1;
}

queue* init_queue(uInt16 size) {
  queue *q = (queue*)calloc(1, sizeof(queue));
  q->front = -1;
  q->rear = -1;
  q->size = size;
  q->arr = (uInt8*)calloc(q->size, sizeof(uInt8));
  return q;
}

void free_queue(queue *q) {
  free(q->arr);
  free(q);
}

bool enque(queue *q, uInt8 data) {
  if (!queue_isfull(q)) {
    q->rear = (q->rear + 1) % q->size;
    q->arr[q->rear] = data;
    if (q->front == -1) {
      q->front = 0;
    }
    return true;
  }
  return false;
}

bool dequeue(queue *q) {
  if (!queue_isempty(q)) {
    if (q->front == q->rear) {
      q->front = -1;
      q->rear = -1;
    } else {
      q->front = (q->front + 1) % q->size;
    }
    return true;
  }
  return false;
}

bool at_queue_front(queue *q, uInt8 *data) {
  if (queue_isempty(q)) {
    return false;
  } else {
    *data = q->arr[q->front];
    return true;
  }
}

bool at_queue_rear(queue *q, uInt8 *data) {
  if (queue_isfull(q)) {
    return false;
  } else {
    *data = q->arr[q->rear];
    return true;
  }
}

uInt16 len_queue(queue *q) {
  uInt16 size;
  if (q->rear > q->front) {
    size = q->rear - q->front + 1;
  } else if (q->rear < q->front) {
    size = (q->rear + q->size) - q->front + 1;
  } else {
    size = 0;
  }
  return size;
}

