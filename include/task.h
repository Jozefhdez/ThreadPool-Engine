#ifndef TASK_H
#define TASK_H

typedef struct Task {
    void *(*func)(void *); // Function pointer
    void *arg;
    int priority;
    int task_id;
} Task;

// function pointer: function pointer to execute
// arg: argument to pass to that function
// priority: higher = more important
// task_id: Unique identifier for task
Task *task_create(void *(*func)(void *), void *arg, int priority, int task_id);
void task_destroy(Task *task);

#endif