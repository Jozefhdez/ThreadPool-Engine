#include "threadpool.h"
#include "lockfree_queue.h"
#include <stdio.h>
#include <stdlib.h>

ThreadPool *threadpool_create(int num_threads) {
    ThreadPool *pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    if (pool == NULL) {
        printf("Error allocating memory for ThreadPool.\n");
        exit(1);
    }

    pool->num_threads = num_threads;
    pool->task_queue = queue_create();
    if (pool->task_queue == NULL) {
        free(pool);
        printf("Error creating task queue.\n");
        exit(1);
    }

    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
    if (threads == NULL) {
        queue_destroy(pool->task_queue);
        free(pool);
        printf("Error allocating memory for Threads.\n");
        exit(1);
    }

    pool->threads = threads;
    pool->shutdown = 0;
    return pool;
}

void threadpool_destroy(ThreadPool *pool) {
    pool->shutdown = 1;
    queue_destroy(pool->task_queue);
    free(pool->threads);
    free(pool);
}

int threadpool_submit(ThreadPool *pool, Task *task) {
    if (pool->shutdown == 1) {
        return -1;
    }

    return queue_push(pool->task_queue, task);
}