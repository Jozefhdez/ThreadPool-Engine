#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "lockfree_queue.h"
#include "task.h"
#include <pthread.h>

typedef struct ThreadPool {
    pthread_t *threads; // array of worker threads IDs
    int num_threads;    // num of worker threads
    int shutdown;       // flag to signal threads to stop (0 == running, 1 ==
                        // shutdown)
    LockFreeQueue *task_queue;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} ThreadPool;

ThreadPool *threadpool_create(int num_threads);
void threadpool_destroy(ThreadPool *pool);
int threadpool_submit(ThreadPool *pool, Task *task); // add task to the queue

#endif