#include "utility.h"
#include "react.h"
#include "demo.h"
#include "debug.h"
#include "mode.h"
#include "guess.h"

//takes a run_mode and calls the appropriate function
int mode_run(run_mode mode, em_State *emotions, qu_queue *notifications) {
	
  switch(mode) {
    case MODE_REACT:
      return rct_main(emotions, notifications);
      
    case MODE_SLEEP:
      return rct_sleep(emotions, notifications);
      
    case MODE_DEMO:
      return demo_main(emotions, notifications);
      
    case MODE_DEBUG:
      return debug_main(emotions, notifications);

    case MODE_GUESS:
      return guess_main(emotions, notifications);

    case MODE_NONE:
      return ERR_NONE;
      
    default:
      return ERR_BAD_MODE;
  } 
}
