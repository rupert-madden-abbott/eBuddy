/* eBuddy demo mode */
#include <stdio.h>
#include <unistd.h>

#include "notify.h"
#include "input.h"
#include "gesture_interface.h"
#include "emotion.h"
#include "config.h"
#include "queue.h"
#include "demo.h"

//Demo mode
int dm_main(em_State *emotions, qu_Queue *notifications, ph_handle *phhandle) {
  nt_Message *message;

  while(1) {
    sleep(1);
    if(qu_size(notifications) > 0) {
      printf("Before POP: %i\n", qu_size(notifications));
      message = qu_pop(notifications);
      printf("After POP: %i\n", qu_size(notifications));
      printf("%s\n", message->text);
      gsi_eyeflash(phhandle);
      gsi_raise_arms(phhandle);
      gsi_sound("computing", 1);
      gsi_printLCD(message->text, phhandle);
      
      fflush(stdout);
    }
      if(in_get_input() == INPT_BATTERY)
	{
            
		gsi_happy_level1(phhandle);
	}
  }


  return 1;
}
