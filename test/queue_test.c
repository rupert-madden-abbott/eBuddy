/**
 * @file   queue_test.c
 * @author Rupert Madden-Abbott
 */

#include <unistd.h>
#include <string.h>
#include "queue.h"
#include "test.h"

void *loop(void *queue);

int main(void) {
  int rc;
  const char *data = "test data";
  char *check;
  qu_Queue   *queue = NULL;

  queue = qu_init();
  if(!queue) fail("Failed to initialise new queue");
  if(queue->size != 0) fail("Failed to set queue size");
  if(queue->head != NULL) fail("Failed to set queue head");
  if(queue->tail != NULL) fail("Failed to set queue tail");
  
  check = qu_pop(queue);
  if(check) fail("Returned data from unpopoluated queue");
  
  rc = qu_push(queue, (void *)data);
  if(rc) fail("Failed to push data on to queue");
  
  rc = qu_size(queue);
  if(rc != 1) fail("Queue size is set incorrectly by push");
  
  check = qu_pop(queue);
  if(!check) fail("Failed to return data from populated queue");
  if(!strcmp(check, data)) fail("Returned data does not match pushed data");
  
  rc = qu_size(queue);
  if(rc != 0) fail("Queue size is set incorrectly by pop");

  qu_free(queue);
  if(queue) fail("Failed to free queue");  

  return 0;
}
