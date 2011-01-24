/**
 * @file   queue_test.c
 * @author Rupert Madden-Abbott
 */
#include <string.h>
#include "queue.h"
#include "test.h"

int main(void) {
  qu_queue *queue = NULL;
  char     name[] = "sdsdsgffgjgjdrd";
  int      length, i;
  
  queue = qu_init();
  if(!queue) fail("Failed to create new queue");

  qu_push(queue, (void *)name[i]);

  qu_pop(queue);

  qu_free(queue);

  return 0;
}
