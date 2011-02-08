/**
 * @file   queue.c
 * @author Rupert Madden-Abbott
*/

#include <stdlib.h>

#include <pthread.h>

#include "utility.h"
#include "queue.h"

qu_Queue *qu_init(void) {
  int rc;
  qu_Queue *queue;
  
  queue = malloc(sizeof(qu_Queue));
  if(!queue) {
    return NULL;
  }
  
  rc = pthread_mutex_init(&queue->mutex, NULL);
  if(rc) {
    return NULL;
  }
  
  queue->size = 0;
  queue->head = queue->tail = NULL;

  return queue;
}

void qu_free(qu_Queue *queue) {
  qu_Node *node;
  
  //While there are still nodes left in the queue, destroy them
  while(queue->size > 0) {
    node = queue->head;
    queue->head = queue->head->link;
    free(node);
    
    queue->size--;
  }
  
  pthread_mutex_destroy(&queue->mutex);
  
  free(queue);  
}

int qu_push(qu_Queue *queue, void *data) {
  int rc;
  
  //Ensure the queue exists
  if(!queue) {
    return UT_ERR_POINTER_NULL;
  }
  
  //Lock the queue for thread safety
  rc = pthread_mutex_trylock(&(queue->mutex));
  if(rc) {
    return UT_ERR_MUTEX_LOCK;
  }
  
  //Check if the queue already contains nodes
  if(!queue->head) {
    //If it doesn't create the first node
    queue->head = (qu_Node *)malloc(sizeof(qu_Node));
    if(!queue->head) {
      rc = pthread_mutex_unlock(&(queue->mutex));
      if(rc) {
        return UT_ERR_MUTEX_UNLOCK;
      }
      return UT_ERR_MEMORY;
    }
    
    //Set the data onto the node
    queue->head->data = data;
    queue->tail = queue->head;
  } 
  else {
    //If it does, add the node to the end
    queue->tail->link = (qu_Node *)malloc(sizeof(qu_Node));
    if(!queue->tail->link) {
      rc = pthread_mutex_unlock(&(queue->mutex));
      if(rc) {
        return UT_ERR_MUTEX_UNLOCK;
      }
      return UT_ERR_MEMORY;
    }
    
    //Set the data onto the node
    queue->tail = queue->tail->link;
    queue->tail->data = data;
  }

  //Increase the size of the queue
  queue->tail->link = NULL;
  queue->size++;
  
  //Allow other threads to access the queue
  rc = pthread_mutex_unlock(&(queue->mutex));
  if(rc) {
    return UT_ERR_MUTEX_UNLOCK;
  }  
  return UT_ERR_NONE;
}

void *qu_pop(qu_Queue *queue) {
  int rc;
  qu_Node *node = NULL;
  void *data = NULL;

  //Ensure the queue exists
  if(!queue) {
    return NULL;
  }
  
  //Lock the queue for thread safety
  rc = pthread_mutex_trylock(&(queue->mutex));
  if(rc) {
    return NULL;
  }
  
  //Ensure the queue contains nodes
  if(queue->size > 0) {
    node = queue->head;
    data = node->data;
    queue->size--;
    //Check if there are nodes left on the queue
    if(queue->size) {
      queue->head = queue->head->link;
    } 
    else {
      queue->head = queue->tail = NULL;
    }
    
    //Allow other threads to access the queue
    rc = pthread_mutex_unlock(&(queue->mutex));
    if(rc) {
      return NULL;
    }
    free(node);
    node = NULL;
    return data;
  }
  
  //Allow other threads to access the queue
  rc = pthread_mutex_unlock(&(queue->mutex));
  if(rc) {
    return NULL;
  }
  
  return NULL;
}

int qu_size(qu_Queue *queue) {
    int size, rc;
    
    rc = pthread_mutex_trylock(&(queue->mutex));
    if(rc) return UT_ERR_UNKNOWN;
    
    size = queue->size;
    rc = pthread_mutex_unlock(&(queue->mutex));
    if(rc) {
      return UT_ERR_MUTEX_UNLOCK;
    }
    return size;
}
