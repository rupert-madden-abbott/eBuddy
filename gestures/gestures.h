#include <stdio.h>
#include <stdlib.h>
#include "phidget21.h"
#include <unistd.h>

#define SERVO_HEAD 0
#define SERVO_TORSO 1
#define SERVO_LEFTARM 2
#define SERVO_RIGHTARM 3

#define STARTPOS_HEAD 140
#define STARTPOS_TORSO 140
#define STARTPOS_ARM 60
#define STARTPOS_ARM 60

extern CPhidgetAdvancedServoHandle gestures_get_servo_handle(void);

int gestures_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p);
//int gestures_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p);
int gestures_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p, int ErrorCode, const char *Description);

int gestures_set_pos(CPhidgetAdvancedServoHandle servo);
int gestures_dance_basic(void);

CPhidgetAdvancedServoHandle gestures_servo_initialise(void);

extern int testy(void);
