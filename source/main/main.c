#include "main.h"
#include "notify.h"
#include "phidget.h"
#include "gesture_interface.h"

int main(void) {
  noti_initialize();
  ph_initialise();
  return 1;
}
