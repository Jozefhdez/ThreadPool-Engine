#include "lockfree_queue.h"
#include "task.h"
#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *print_task(void *arg) {
    int *num = (int *)arg;
    printf("Task executed with value: %d\n", *num);
    return NULL;
}

int main() {
    ThreadPool *pool = threadpool_create(2);
    printf("ThreadPool created with %d worker threads\n", pool->num_threads);

    // Submit 5 tasks
    int vals[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        Task *t = task_create(print_task, &vals[i], i, i);
        threadpool_submit(pool, t);
        printf("Submitted task %d\n", i);
    }

    // Give workers time to process
    sleep(1);

    printf("Shutting down...\n");
    threadpool_destroy(pool);
    printf("ThreadPool destroyed\n");
    return 0;
}