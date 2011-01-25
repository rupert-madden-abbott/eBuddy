/**
 * @file   queue_test.c
 * @author Rupert Madden-Abbott
 */
#include "queue.h"
#include "test.h"

int main(void) {
  qu_queue *queue = NULL;
  char     *first = "hello";
  char     *second = "goodbye";
  
  queue = qu_init();
  if(!queue) fail("Failed to create new queue");

  qu_push(queue, first);
  qu_push(queue, second);
    qu_free(queue);
  
  qu_pop(queue);
  /*printf("%s", (char *) qu_pop(queue));
    printf("%s", (char *) qu_pop(queue));
  */


  return 0;
}
