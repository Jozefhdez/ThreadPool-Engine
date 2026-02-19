#include "lockfree_queue.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>

void *print_task(void *arg) {
    int *num = (int *)arg;
    printf("Task executed with value: %d\n", *num);
    return NULL;
}

int main() {
    LockFreeQueue *queue = queue_create();

    // Create 3 tasks
    int vals[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        Task *t = task_create(print_task, &vals[i], i, i);
        queue_push(queue, t);
    }

    printf("Queue size: %d\n", queue->size);

    // Pop and execute all tasks
    Task *t;
    while ((t = queue_pop(queue)) != NULL) {
        t->func(t->arg);
        task_destroy(t);
    }

    printf("Queue size after: %d\n", queue->size);
    queue_destroy(queue);
    return 0;
}