#ifndef GESTURES_H
#define GESTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <phidget21.h>

#include "utility.h"
#include "config.h"

// LCD
#define LCDWIDTH 20
#define SCROLLSPEED 1500000
#define WHITESPACE 32  // ascii code for whitespace

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


//defined servo phidget library fucntions
#define GS_SETPOS(a, b, c) error = CPhidgetAdvancedServo_setPosition (a, b, c)
#define GS_SETENG(a, b, c) error = CPhidgetAdvancedServo_setEngaged(a, b, c)
#define GS_SETACC(a, b, c) error = CPhidgetAdvancedServo_setAcceleration(a, b, c)
#define GS_SETVEL(a, b, c) error = CPhidgetAdvancedServo_setVelocityLimit(a, b, c)

#define GS_RIGHTEYE 0
#define GS_LEFTEYE 2


CPhidgetAdvancedServoHandle gs_get_servo_handle(void);

int gs_set_pos(CPhidgetAdvancedServoHandle servo);
extern int gs_dance_basic(CPhidgetAdvancedServoHandle servo);
int gs_sound(const char *sound, int repeat);
extern int gs_dance_advanced(CPhidgetAdvancedServoHandle servo);
extern int gs_raise_arms(CPhidgetAdvancedServoHandle servo);
extern int gs_shake_head(CPhidgetAdvancedServoHandle servo);
extern int gs_move_arms(CPhidgetAdvancedServoHandle servo);
extern int gs_turn(CPhidgetAdvancedServoHandle servo);
extern int gs_eyeson(CPhidgetInterfaceKitHandle ifkit);
extern int gs_eyesoff(CPhidgetInterfaceKitHandle ifkit);
extern int gs_eyeflash(CPhidgetInterfaceKitHandle ifkit);
extern int gs_wave_left(CPhidgetAdvancedServoHandle servo);
extern int gs_wave_right(CPhidgetAdvancedServoHandle servo);
extern int gs_rapid_eyeflash(CPhidgetInterfaceKitHandle ifkit);
extern int gs_raise_right(CPhidgetAdvancedServoHandle servo);
extern int gs_raise_left(CPhidgetAdvancedServoHandle servo);

/*
 * Takes a string as input, along with the LCD handle, and prints the string
 * to the LCD screen. Prints two lines at a time. Exits when the string has 
 * been printed.
 */
int gs_printstring(const char* str,CPhidgetTextLCDHandle txt_lcd);
#endif
