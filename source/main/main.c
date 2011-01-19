#include "main.h"
#include "notify.h"
#include "phidget.h"
#include "gesture_interface.h"

int main(void) {
	gsi_happy_level1();
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
