#define _GNU_SOURCE
#include "threadpool.h"
#include "lockfree_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void *worker_thread(void *arg) {
    ThreadPool *pool = (ThreadPool *)arg;
    while (1) {
        // protect shutdown flag and queue access with the pool mutex.
        pthread_mutex_lock(&pool->lock);

        // wait until there is work to do or shutdown is requested.
        // pthread_cond_wait unlocks the mutex while sleeping and re-locks it
        // before returning
        while (pool->task_queue->size == 0 && pool->shutdown == 0) {
            pthread_cond_wait(&pool->cond, &pool->lock);
        }

        // exit only when shutdown is requested and   is no pending work
        if (pool->shutdown == 1 && pool->task_queue->size == 0) {
            pthread_mutex_unlock(&pool->lock);
            return NULL;
        }

        // pop one task while holding the lock to keep queue state consistent
        Task *task = queue_pop(pool->task_queue);
        pthread_mutex_unlock(&pool->lock);

        // execute outside the lock so other threads can keep
        // submitting/popping.
        if (task != NULL) {
            task->func(task->arg);
            task_destroy(task);
        }
    }
}

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
    int ret;
    ret = pthread_mutex_init(&pool->lock, NULL);
    if (ret != 0) {
        printf("pthread_mutex_init failed: %s\n", strerror(ret));
        queue_destroy(pool->task_queue);
        free(pool->threads);
        free(pool);
        return NULL;
    }

    ret = pthread_cond_init(&pool->cond, NULL);
    if (ret != 0) {
        printf("pthread_cond_init failed: %s\n", strerror(ret));
        pthread_mutex_destroy(&pool->lock);
        queue_destroy(pool->task_queue);
        free(pool->threads);
        free(pool);
        return NULL;
    }

    for (int i = 0; i < pool->num_threads; i++) {
        int result = pthread_create(&pool->threads[i], NULL, worker_thread,
                                    (void *)pool);
        if (result != 0) {
            printf("Error creating thread %d.\n", i);
            // wake and stop already-created workers before cleanup
            pthread_mutex_lock(&pool->lock);
            pool->shutdown = 1;
            pthread_cond_broadcast(&pool->cond);
            pthread_mutex_unlock(&pool->lock);

            for (int j = 0; j < i; j++) {
                pthread_join(pool->threads[j], NULL);
            }

            pthread_cond_destroy(&pool->cond);
            pthread_mutex_destroy(&pool->lock);
            queue_destroy(pool->task_queue);
            free(pool->threads);
            free(pool);
            return NULL;
        }
    }
    return pool;
}

void threadpool_destroy(ThreadPool *pool) {
    // request shutdown and wake all sleeping workers
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->cond);
    pthread_mutex_unlock(&pool->lock);

    // wait for workers to finish before destroying shared resources
    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_cond_destroy(&pool->cond);
    pthread_mutex_destroy(&pool->lock);
    queue_destroy(pool->task_queue);
    free(pool->threads);
    free(pool);
}

int threadpool_submit(ThreadPool *pool, Task *task) {
    pthread_mutex_lock(&pool->lock);

    if (pool->shutdown == 1) {
        pthread_mutex_unlock(&pool->lock);
        return -1;
    }

    int result = queue_push(pool->task_queue, task);
    if (result == 0) {
        // wake one worker because new work is available
        pthread_cond_signal(&pool->cond);
    }

    pthread_mutex_unlock(&pool->lock);
    return result;
}