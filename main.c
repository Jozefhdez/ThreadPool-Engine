#include "task.h"
#include <stdio.h>
#include <stdlib.h>

void *print_task(void *arg) {
    int *num = (int *)arg;
    printf("Task executed with value: %d\n", *num);
    return NULL;
}

int main() {
    int value = 67;
    Task *task = task_create(print_task, &value, 5, 1);

    task->func(task->arg);

    task_destroy(task);
    return 0;
}