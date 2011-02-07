/**
 * @file   queue.c
 * @author Rupert Madden-Abbott
*/

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
  int rc;
  
  rc = pthread_mutex_trylock(&(queue->mutex));
  if(rc) return UT_ERR_UNKNOWN;
  
  if(!queue->head) {
    queue->head = (qu_node *)malloc(sizeof(qu_node));
    if(!queue->head) {
      pthread_mutex_unlock(&(queue->mutex));
      return UT_ERR_UNKNOWN;
    }
    
    queue->head->data = data;
    queue->tail = queue->head;
  } 
  else {
    queue->tail->link = (qu_node *)malloc(sizeof(qu_node));
    if(!queue->tail->link) {
      pthread_mutex_unlock(&(queue->mutex));
      return UT_ERR_UNKNOWN;
    }
    
    queue->tail = queue->tail->link;
    queue->tail->data = data;
  }

  queue->tail->link = NULL;
  queue->size++;
  
  pthread_mutex_unlock(&(queue->mutex));
  
  return UT_ERR_NONE;
}

void *qu_pop(qu_queue *queue) {
  int rc;
  qu_node *node = NULL;

  if(!queue) return NULL;
  
  rc = pthread_mutex_trylock(&(queue->mutex));
  if(rc) return NULL;
  
  if(queue->size > 0) {
    node = queue->head;
    queue->size--;
    if(queue->size) {
      queue->head = queue->head->link;
    } 
    else {
      queue->head = queue->tail = NULL;
    }
    
    pthread_mutex_unlock(&(queue->mutex));
    return node->data;
  }
  
  pthread_mutex_unlock(&(queue->mutex));
  
  return NULL;
}

int qu_size(qu_queue *queue) {
    int size, rc;
    
    rc = pthread_mutex_trylock(&(queue->mutex));
    if(rc) return UT_ERR_UNKNOWN;
    
    size = queue->size;
    pthread_mutex_unlock(&(queue->mutex));
    return size;
}
