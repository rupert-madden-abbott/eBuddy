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
  in_input_type input_event;

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

    input_event = in_get_input();

      if(input_event == INPT_BATTERY) {
        gsi_happy_level1(phhandle);
      }

      else if(input_event == INPT_DEMO) {
        gsi_eyeflash(phhandle);
        gsi_printLCD("Exit demo", phhandle);    
        return(UT_ERR_NONE);
      }

      else if(input_event == INPT_POWER_OFF) {    
        return(UT_ERR_CLOSE);
      }
    
  }


  return 1;
}
