#include "mode.h"

//takes a run_mode and calls the appropriate function
int mode_run(run_mode mode, em_State *emotions, qu_queue *notifications, ph_handle *phhandle) {
	
  switch(mode) {
    case MODE_REACT:
      return rct_main(emotions, notifications, phhandle);
      
    case MODE_SLEEP:
      return rct_sleep(emotions, notifications, phhandle);
      
    case MODE_DEMO:
      return demo_main(emotions, notifications, phhandle);
      
    case MODE_DEBUG:
      return debug_main(emotions, notifications, phhandle);

    case MODE_GUESS:
      return guess_main(emotions, notifications, phhandle);

    case MODE_NONE:
      return ERR_NONE;
      
    default:
      return ERR_BAD_MODE;
  } 
}
