/**
 * @file   queue_test.c
 * @author Rupert Madden-Abbott
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "utility.h"
#include "queue.h"
#include "test.h"

void *loop(void *queue);

int main(void) {
  qu_queue   *queue = NULL;
  pthread_t thread;

  queue = qu_init();
  if(!queue) fail("Failed to create new queue");

  pthread_create(&thread, NULL, loop, queue);

  
  while(1) {
    sleep(1);
    printf("%s", (char *) qu_pop(queue));
    fflush(stdout);
  }

  qu_free(queue);

  pthread_exit(NULL);
}

void *loop(void *queue) {
  char first[] = "hello";
  char second[] = "goodbye";
  
  while(1) {
    sleep(1);
    qu_push(queue, first);
    qu_push(queue, second);
  }
  
  pthread_exit(NULL);
}
