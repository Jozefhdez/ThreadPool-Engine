#include "lockfree_queue.h"
#include "task.h"
#include <stdio.h>
#include <stdlib.h>

LockFreeQueue *queue_create() {
    LockFreeQueue *queue = (LockFreeQueue *)malloc(sizeof(LockFreeQueue));
    if (queue == NULL) {
        printf("Error allocating memory for LockFreeQueue.\n");
        return NULL;
    }

    // sentinel node
    QueueNode *dummy = (QueueNode *)malloc(sizeof(QueueNode));
    if (dummy == NULL) {
        printf("Error allocating memory for sentinel node.\n");
        free(queue);
        return NULL;
    }
    dummy->task = NULL;
    dummy->next = NULL;

    // make both head and tail point to dummy
    queue->head = dummy;
    queue->tail = dummy;
    queue->size = 0;

    return queue;
}

int queue_push(LockFreeQueue *queue, Task *task) {
    QueueNode *new_node = (QueueNode *)malloc(sizeof(QueueNode));
    if (new_node == NULL) {
        printf("Error allocating memory for node.\n");
        return -1;
    }

    new_node->task = task;
    new_node->next = NULL;
    queue->tail->next = new_node;
    queue->tail = new_node;
    queue->size++;

    return 0;
}

Task *queue_pop(LockFreeQueue *queue) {
    if (queue->head->next == NULL) {
        return NULL;
    }

    QueueNode *old_head = queue->head;
    Task *task = queue->head->next->task;
    queue->head = queue->head->next;

    free(old_head);
    queue->size--;
    return task;
}

void queue_destroy(LockFreeQueue *queue) {
    while (queue->head->next != NULL) {
        QueueNode *temp = queue->head->next;
        queue->head = queue->head->next;
        task_destroy(temp->task);
        free(temp);
    }

    free(queue->head); // free dummy node
    free(queue);
}