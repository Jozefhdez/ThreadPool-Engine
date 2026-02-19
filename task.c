#include "task.h"
#include <stdio.h>
#include <stdlib.h>

Task *task_create(void *(*func)(void *), void *arg, int priority, int task_id) {
    Task *task = (Task *)malloc(sizeof(Task));
    if (task == NULL) {
        printf("Error allocating memory for task.");
        exit(1);
    }

    task->func = func;
    task->arg = arg;
    task->priority = priority;
    task->task_id = task_id;

    return task;
}

void task_destroy(Task *task) {
    free(task);
}