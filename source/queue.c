/**
 * @file   queue.c
 * @author Rupert Madden-Abbott
*/

#include "utility.h"
#include "queue.h"

qu_queue *qu_init(void) {
  qu_queue *queue = malloc(sizeof(qu_queue));
  
  if(!queue) return NULL;
  
  queue->size = 0;
  queue->head = queue->tail = NULL;

  return queue;
}

void qu_free(qu_queue *queue) {
  qu_node *node;
  
  while(queue->size > 0) {
    node = queue->head;
    queue->head = queue->head->link;
    free(node);
    
    queue->size--;
  }
  
  free(queue);  
}

int qu_push(qu_queue *queue, void *data) {
  if(!queue->head) {
    queue->head = (qu_node *)malloc(sizeof(qu_node));
    if(!queue->head) return ERR_UNKNOWN;
    
    queue->head->data = data;
    queue->tail = queue->head;
  } 
  else {
    queue->tail->link = (qu_node *)malloc(sizeof(qu_node));
    if(!queue->tail->link) return ERR_UNKNOWN;
    
    queue->tail = queue->tail->link;
    queue->tail->data = data;
  }

  queue->tail->link = NULL;
  queue->size++;
  
  return ERR_NONE;
}

void *qu_pop(qu_queue *queue) {
  qu_node *node;

  if(!queue) return NULL;
  
  if(queue->size > 0) {
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
