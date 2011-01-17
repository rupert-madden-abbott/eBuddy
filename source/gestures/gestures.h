#include "eBuddy.h"
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

#define SETPOS(a, b, c) CPhidgetAdvancedServo_setPosition (a, b, c)
#define SETENG(a, b, c) CPhidgetAdvancedServo_setEngaged(a, b, c);
#define SETACC(a, b, c) CPhidgetAdvancedServo_setAcceleration(a, b, c)
#define SETVEL(a, b, c) CPhidgetAdvancedServo_setVelocityLimit(a, b, c);

CPhidgetAdvancedServoHandle gestures_get_servo_handle(void);

int gs_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p);
//int gestures_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p);
int gs_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p, int ErrorCode, const char *Description);

int gs_pos(CPhidgetAdvancedServoHandle servo);
extern int gs_dance_basic(void);
extern int gs_sound(int sound, int itineration);

extern CPhidgetAdvancedServoHandle gs_servo_initialise(void);


