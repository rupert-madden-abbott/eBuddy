#include <string.h>
#include <curl/curl.h>
#include <pthread.h>
#include <unistd.h>
#include <oauth.h>
#include <ctype.h>
#include <string.h>
#include <jansson.h>
#include "utility.h"
#include "config.h"
#include "queue.h"
#include "notify.h"
#include "test.h"

int main(void) {
  qu_queue *queue = qu_init();
  pthread_t thread;
  nt_message *msg;

  pthread_create(&thread, NULL, nt_poll, queue);
  
  while(1) {
    sleep(1);
    if(qu_size(queue) > 0) {
      msg = qu_pop(queue);
      printf("%s", msg->text);
      fflush(stdout);
    }
  }
 
  pthread_exit(NULL);
}
