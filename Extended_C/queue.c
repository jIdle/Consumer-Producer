//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Queue routines
//
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

queue_t *create_queue(int bufsize) {
  queue_t *q = (queue_t *) malloc(sizeof(queue_t));
  q->buffer = (int *) malloc(sizeof(int) * bufsize);
  q->capacity = bufsize;
  q->head = q->tail = q->size = 0;
  return q;
}

void add_item(queue_t *q, int val) {
  assert(q->size < q->capacity);  // make sure queue is not full
  q->buffer[q->tail] = val;
  q->tail = (q->tail + 1) % q->capacity;
  q->size++;
}

int remove_item(queue_t *q) {
  assert(q->size > 0);           // make sure queue is not empty
  int val = q->buffer[q->head];  
  q->head = (q->head + 1) % q->capacity;
  q->size--;
  return val;
}
