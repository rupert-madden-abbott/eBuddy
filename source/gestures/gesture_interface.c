#include "gesture_interface.h"

int gsi_happy_level1(void)
{
    gs_dance_basic();
    return 0;
}

int gsi_initialise(void)
{
    gs_servo_initialise();
    return 0;
}
