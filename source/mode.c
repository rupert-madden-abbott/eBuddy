#include "react.h"
#include "guess.h"
#include "debug.h"
#include "demo.h"
#include "mode.h"

//takes a run_mode and calls the appropriate function
int md_run(md_mode mode, em_State *emotions, qu_queue *notifications, ph_handle *phhandle) {
	
  switch(mode) {
    case MODE_REACT:
      return rc_main(emotions, notifications, phhandle);
      
    case MODE_SLEEP:
      return rc_sleep(emotions, notifications, phhandle);
      
    case MODE_DEMO:
      return dm_main(emotions, notifications, phhandle);
      
    case MODE_DEBUG:
      return db_main(emotions, notifications, phhandle);

    case MODE_GUESS:
      return guess_main(emotions, notifications, phhandle);

    case MODE_NONE:
      return UT_ERR_NONE;
      
    default:
      return UT_ERR_BAD_MODE;
  } 
}
