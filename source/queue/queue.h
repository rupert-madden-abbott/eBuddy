/**
 * @file   queue.h
 * @author Rupert Madden-Abbott
*/

#ifndef QUEUE_H
#define QUEUE_H

typedef struct qu_node {
    void *data;
    struct qu_node *link;
} qu_node;

typedef struct qu_queue {
    int size;
    qu_node *head;
    qu_node *tail;
} qu_queue;

void qu_init(qu_queue *queue);
int queue_push(qu_queue *queue, void *data);
void *queue_pop(Queue *q, int release);

#endif

