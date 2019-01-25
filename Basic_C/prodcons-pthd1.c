/*
 * Kobe Davis
 * Prof. Li
 * CS 415P
 * 22 January 2019
 */

#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include "queue.h"

pthread_mutex_t lock;
pthread_cond_t proWait;
pthread_cond_t conWait;
int capacity = 20;
int items = 100;

int isEmpty(queue_t * Q) {
    if(!Q->size)
        return 1;
    return 0;
}

int isFull(queue_t * Q) {
    if(Q->size == Q->capacity)
        return 1;
    return 0;
}

void * consumer(void * vArg) {
    int removed = 0;
    queue_t * pcQueue = (queue_t*)vArg;
    printf("Consumer starting on Core-%d\n", sched_getcpu());

    while(items > 0 || removed < 100) {
        pthread_mutex_lock(&lock);
        while(isEmpty(pcQueue)){
            pthread_cond_wait(&conWait, &lock);
        }
        removed = remove_item(pcQueue);
        printf("Consumer removed value %d (qsize=%d)\n", removed, pcQueue->size);
        pthread_cond_signal(&proWait);
        pthread_mutex_unlock(&lock);
    }
}

int main() {
    queue_t * pcQueue = create_queue(capacity); 

    pthread_t cThread;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&proWait, NULL);
    pthread_cond_init(&conWait, NULL);
    pthread_create(&cThread, NULL, consumer, (void*)pcQueue);

    printf("Producer starting on Core-%d\n", sched_getcpu()); 

    while(items--) {
        pthread_mutex_lock(&lock);
        while(isFull(pcQueue))
            pthread_cond_wait(&proWait, &lock);
        add_item(pcQueue, (100 - items));
        printf("Producer added value %d (qsize=%d)\n", (100 - items), pcQueue->size);
        pthread_cond_signal(&conWait);
        pthread_mutex_unlock(&lock);
    }
    pthread_join(cThread, NULL);
    printf("Produced 100 items, joining consumer thread.\n");

    return 0;
}
