#ifndef GESTURES_H
#define GESTURES_H
#include "phidget.h"
#include "config.h"


#define LCDWIDTH 20
#define SCROLLSPEED 1500000
#define WHITESPACE 32

//servo mappings
#define GS_SERVO_HEAD 0
#define GS_SERVO_TORSO 1
#define GS_SERVO_LEFTARM 2
#define GS_SERVO_RIGHTARM 3

//default servo postions
#define GS_STARTPOS_HEAD 110
#define GS_STARTPOS_TORSO 110
#define GS_STARTPOS_RIGHTARM 200
#define GS_STARTPOS_LEFTARM 20

#define GS_MICRO 1000

//defined servo phidget library fucntions
#define GS_SETPOS(a, b, c) CPhidgetAdvancedServo_setPosition (a, b, c)
#define GS_SETENG(a, b, c) CPhidgetAdvancedServo_setEngaged(a, b, c)
#define GS_SETACC(a, b, c) CPhidgetAdvancedServo_setAcceleration(a, b, c)
#define GS_SETVEL(a, b, c) CPhidgetAdvancedServo_setVelocityLimit(a, b, c)

#define GS_RIGHTEYE 0
#define GS_LEFTEYE 2

#define MP3PLAYERCMD "mpg123 -q "

CPhidgetAdvancedServoHandle gs_get_servo_handle(void);

int gs_init(PhidgetHandle *phidgets, const char *config);
int gs_set_pos(CPhidgetAdvancedServoHandle servo);
extern int gs_dance_basic(CPhidgetAdvancedServoHandle servo);
extern int gs_sound(int sound, int itineration);
extern int gs_dance_advanced(CPhidgetAdvancedServoHandle servo);
extern int gs_raise_arms(CPhidgetAdvancedServoHandle servo);
extern int gs_shake_head(CPhidgetAdvancedServoHandle servo);
extern int gs_move_arms(CPhidgetAdvancedServoHandle servo);
extern int gs_turn(CPhidgetAdvancedServoHandle servo);
extern int gs_eyeson(CPhidgetInterfaceKitHandle ifkit);
extern int gs_eyesoff(CPhidgetInterfaceKitHandle ifkit);
extern int gs_eyeflash(CPhidgetInterfaceKitHandle ifkit);


int gs_printstring(char* str,CPhidgetTextLCDHandle txt_lcd);
#endif
