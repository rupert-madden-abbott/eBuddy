/**
 * @file   queue.c
 * @author Rupert Madden-Abbott
*/

#include <stdlib.h>
#include "queue.h"

void qu_init(qu_queue *queue) {
  queue->size = 0;
  queue->head = q->tail = NULL;
}

int qu_push(qu_queue *queue, void *data) {
  if(!queue->head) {
    queue->head = (qu_node *)malloc(sizeof(qu_node));
    if(!queue->head) return -1;
    
    queue->head->data = element;
    queue->tail = q->head;
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
  qu_node *old;
  
  if (queue->size) {
    old = queue->head;
    queue->size--;
    if(queue->size) {
      queue->head = queue->head->link;
    } 
    else {
      queue->head = queue->tail = NULL;
    }
  return old->data;
  }
  
  return NULL;
}
