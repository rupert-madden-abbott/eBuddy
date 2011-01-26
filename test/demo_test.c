#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <phidget21.h>
#include <jansson.h>
#include <pthread.h>

#include "input.h"
#include "utility.h"
#include "queue.h"
#include "notify.h"
#include "phidget.h"
#include "gesture_interface.h"
#include "emotion.h"
#include "config.h"
#include "main.h"

int main(void) {
  int input;
  em_State *emotions;
  error_code rc;
  qu_queue *queue = qu_init();
  nt_message *message = NULL;
  pthread_t thread;
  
  //initialise the phidgets
  rc = ph_init(CONFIG_PATH);
  if(rc) {
  	printf("Error initialising phidgits\n");
  	exit(1);
  }
  
  //initialise notification system
  rc = nt_init(queue, "conf/notify.json");
  if(rc) {
  	printf("Error initialising notifications\n");
  	exit(1);
  }

  //pthread_create(&thread, NULL, nt_poll, queue);
  
  while(1) {
    sleep(1);
    if(qu_size(queue) > 0) {
      printf("Before POP: %i\n", qu_size(queue));
      message = qu_pop(queue);
      printf("After POP: %i\n", qu_size(queue));
      printf("%s\n", message->text);
      gsi_eyeflash();
      gsi_raise_arms();
      gsi_notification();
      gsi_printLCD(message->text);
      
      fflush(stdout);
    }
      if(in_get_input()==battery)
	{
            
		gsi_happy_level1();
	}
  }


  return 1;
}
