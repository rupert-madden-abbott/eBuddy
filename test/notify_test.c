#include "queue.h"
#include "notify.h"

int main(void) {
  int rc;
  qu_queue *queue = qu_init();
  pthread_t thread;
  nt_message *msg;

  rc = nt_init(queue, "conf/notify.json");
  if(rc) {
    printf("%i\n", rc);
    return 1;
  }
  
  while(1) {
    sleep(1);
    if(qu_size(queue) > 0) {
      msg = qu_pop(queue);
      if(msg) {
        if(msg->error) {
          printf("Thread threw an error\n");
          return 0;
        }
      printf("%s", msg->text);
      fflush(stdout);
      }
    }
  }
 
  pthread_exit(NULL);
}
