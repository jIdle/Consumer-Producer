//----------------------------------------------------------------------------- 
// Program code for CS 415/515 Parallel Programming, Portland State University.
//----------------------------------------------------------------------------- 

// Queue representation
//

typedef struct queue_ queue_t;
struct queue_ {
  int *buffer;   // a circular buffer
  int capacity;  // buffer capacity
  int head;      // head of queue
  int tail;      // tail of queue
  int size;	 // current size of queue
}; 

extern queue_t *create_queue(int bufsize);
extern void add_item(queue_t *q, int val);
extern int remove_item(queue_t *q);
