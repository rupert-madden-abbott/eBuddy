#include "main.h"
#include "notify.h"
#include "phidget.h"
#include "gesture_interface.h"

int main(void) {
  noti_initialize();
  ph_initialise();
  main_loop();
  return 0;
}


int main_loop(void)
{
    while(1){


    }
    return 0;
}

void end(void)
{
	ph_destruct();
    exit(0);
}
