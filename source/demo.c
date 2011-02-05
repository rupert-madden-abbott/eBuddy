/* eBuddy demo mode */

#include "demo.h"

//Demo mode
int demo_main(em_State *emotions, qu_queue *notifications) {
  nt_message *message;

  while(1) {
    sleep(1);
    if(qu_size(notifications) > 0) {
      printf("Before POP: %i\n", qu_size(notifications));
      message = qu_pop(notifications);
      printf("After POP: %i\n", qu_size(notifications));
      printf("%s\n", message->text);
      gsi_eyeflash();
      gsi_raise_arms();
      gsi_notification();
      gsi_printLCD(message->text);
      
      fflush(stdout);
    }
      if(in_get_input() == INPT_BATTERY)
	{
            
		gsi_happy_level1();
	}
  }


  return 1;
}
