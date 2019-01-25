/*
 * Kobe Davis
 * Prof. Li
 * CS 415P
 * 22 January 2019
 */

#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

pthread_mutex_t lock;
pthread_cond_t proWait;
pthread_cond_t conWait;
queue_t * pcQueue;
int capacity;
int items;
int removed;
int end;

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
    int tid = *((int*)vArg);
    int * count = (int*)vArg;
    *count = 0;
    printf("Consumer[%d] starting on Core-%d\n", tid, sched_getcpu());

    while(items > 0 || removed < 100) {
        pthread_mutex_lock(&lock);
        while(isEmpty(pcQueue) && removed < 100)
            pthread_cond_wait(&conWait, &lock);
        if(removed < 100) { // Check again for those stuck in condition loop
            printf("Consumer[%d] removed value %d (qsize=%d)\n", tid, remove_item(pcQueue), pcQueue->size);
            ++removed;
            ++(*count);
        }
        pthread_cond_signal(&proWait);
        pthread_mutex_unlock(&lock);
    }
    printf("Consumer[%d] has completed. Exiting...\n", tid);
    end = 1;
    pthread_cond_broadcast(&conWait);
}

int main(int argc, char ** argv) {
    int tNum = 1;
    if(argc > 1)
        tNum = atoi(argv[1]);
    capacity = 20;
    items = 100;
    removed = 0;
    end = 0;
    int * count = malloc(sizeof(int) * tNum);
    pcQueue = create_queue(capacity); 
    pthread_t cThreads[tNum];

    for(int i = 0; i < tNum; ++i)
        count[i] = i;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&proWait, NULL);
    pthread_cond_init(&conWait, NULL);

    for(int i = 0; i < tNum; ++i) 
        pthread_create(&cThreads[i], NULL, consumer, (void*)&(count[i]));

    printf("Producer starting on Core-%d\n", sched_getcpu()); 

    while(items--) {
        pthread_mutex_lock(&lock);
        while(isFull(pcQueue))
            pthread_cond_wait(&proWait, &lock);
        add_item(pcQueue, (100 - items));
        printf("Producer added value %d (qsize=%d)\n", (100 - items), pcQueue->size);
        pthread_cond_broadcast(&conWait);
        pthread_mutex_unlock(&lock);
    }
    for(int i = 0; i < tNum; ++i)
        pthread_join(cThreads[i], NULL);
    printf("Produced 100 items, joining consumer threads.\n");

    int sum = 0;
    for(int i = 0; i < tNum; ++i) {
        printf("C[%d]: %d\n", i, count[i]);
        sum += count[i];
    }
    printf("Total items across threads: %d\n", sum);

    return 0;
}
