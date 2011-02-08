#include "gesture.h"
#include "test.h"

int main(void) {

  CPhidgetAdvancedServoHandle servo = 0;  //create phidget handles
  CPhidgetInterfaceKitHandle  ifkit = 0;
  CPhidgetAdvancedServo_create(&servo);
  CPhidget_open((CPhidgetHandle)servo, -1);
  CPhidgetInterfaceKit_create(&ifkit);
  CPhidget_open((CPhidgetHandle)ifkit, -1);

  getchar();
  printf("%d\n", gs_set_pos(servo));
  getchar();
  printf("%d\n", gs_dance_basic(servo));
  getchar();
  printf("%d\n", gs_dance_advanced(servo));
  getchar();
  printf("%d\n", gs_raise_arms(servo));
  getchar();
  printf("%d\n", gs_shake_head(servo));
  getchar();
  printf("%d\n", gs_move_arms(servo));
  getchar();
  printf("%d\n", gs_turn(servo));
  getchar();
  printf("%d\n", gs_wave_left(servo));
  getchar();
  printf("%d\n", gs_wave_right(servo));
  getchar();
  printf("%d\n", gs_raise_right(servo));
  getchar();
  printf("%d\n", gs_raise_left(servo));
  getchar();
  printf("%d\n", gs_eyeson(ifkit));
  getchar();
  printf("%d\n", gs_eyesoff(ifkit));
  getchar();
  printf("%d\n", gs_eyeflash(ifkit));
  getchar();
  printf("%d\n", gs_rapid_eyeflash(ifkit));
  getchar();

  CPhidget_close((CPhidgetHandle)servo);
  getchar();
  printf("%d\n", gs_dance_basic(servo)); // should throw error as phidget 
  return 0;
}
