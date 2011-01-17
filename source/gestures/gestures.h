#include "phidget.h"

#define GS_SERVO_HEAD 0
#define GS_SERVO_TORSO 1
#define GS_SERVO_LEFTARM 2
#define GS_SERVO_RIGHTARM 3

#define GS_STARTPOS_HEAD 140
#define GS_STARTPOS_TORSO 140
#define GS_STARTPOS_ARM 60
#define GS_STARTPOS_ARM 60

#define GS_SETPOS(a, b, c) CPhidgetAdvancedServo_setPosition (a, b, c)
#define GS_SETENG(a, b, c) CPhidgetAdvancedServo_setEngaged(a, b, c)
#define GS_SETACC(a, b, c) CPhidgetAdvancedServo_setAcceleration(a, b, c)
#define GS_SETVEL(a, b, c) CPhidgetAdvancedServo_setVelocityLimit(a, b, c)

CPhidgetAdvancedServoHandle gs_get_servo_handle(void);

int gs_set_pos(CPhidgetAdvancedServoHandle servo);
extern int gs_dance_basic(CPhidgetAdvancedServoHandle servo);
extern int gs_sound(int sound, int itineration);
extern int gs_dance_advanced(CPhidgetAdvancedServoHandle servo);
extern int gs_raise_arms(CPhidgetAdvancedServoHandle servo);





