#ifndef LOCK_FREE_QUEUE_h
#define LOCK_FREE_QUEUE_h
#include "task.h"

typedef struct QueueNode {
    Task *task;
    struct QueueNode *next;
} QueueNode;

typedef struct LockFreeQueue {
    QueueNode *head;
    QueueNode *tail;
    int size;
} LockFreeQueue;

LockFreeQueue *queue_create();
int queue_push(LockFreeQueue *queue, Task *task);
Task *queue_pop(LockFreeQueue *queue);
void queue_destroy(LockFreeQueue *queue);

#endif