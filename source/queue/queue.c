/**
 * @file   queue.c
 * @author Rupert Madden-Abbott
*/

#include "queue.h"

qu_queue *qu_init(void) {
  qu_queue *queue;
  
  queue->size = 0;
  queue->head = q->tail = NULL;

  return queue;
}

int qu_push(qu_queue *queue, void *data) {
  if(!queue->head) {
    queue->head = (qu_node *)malloc(sizeof(qu_node));
    if(!queue->head) return -1;
    
    queue->head->data = element;
    queue->tail = queue->head;
  } 
  else {
    queue->tail->link = (qu_node *)malloc(sizeof(qu_node));
    if(!queue->tail->link) return -1;
    
    queue->tail = queue->tail->link;
    queue->tail->data = data;
  }

  queue->tail->link = NULL;
  queue->size++;
  
  return 0;
}

void *queue_pop(qu_queue *queue) {
  qu_node *node;
  
  if (queue->size) {
    node = queue->head;
    queue->size--;
    if(queue->size) {
      queue->head = queue->head->link;
    } 
    else {
      queue->head = queue->tail = NULL;
    }
  return node->data;
  }
  
  return NULL;
}
