#include "lockfree_queue.h"
#include "task.h"
#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>

void *print_task(void *arg) {
    int *num = (int *)arg;
    printf("Task executed with value: %d\n", *num);
    return NULL;
}

int main() {
    ThreadPool *pool = threadpool_create(4);
    printf("ThreadPool created with %d threads\n", pool->num_threads);

    int vals[] = {100, 200, 300};
    for (int i = 0; i < 3; i++) {
        Task *t = task_create(print_task, &vals[i], i, i);
        if (threadpool_submit(pool, t) == 0) {
            printf("Task %d submitted\n", i);
        }
    }

    printf("Queue has %d tasks\n", pool->task_queue->size);

    // Manually execute tasks (since we don't have workers yet)
    Task *t;
    while ((t = queue_pop(pool->task_queue)) != NULL) {
        t->func(t->arg);
        task_destroy(t);
    }

    threadpool_destroy(pool);
    printf("ThreadPool destroyed\n");
    return 0;
}