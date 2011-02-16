/**
 *@file gesture.h
 *@author Andrew Barber & Andrew Pickin (LCD code)
 *
 *The gesture module contains all output to the phidgets made via calls to the
 *phidget library.  All functions take an argument of the relevant 
 *CPhidgetHandle and return a phidget error code greater than zero.  This
 *module also contains code to play sounds. 
 */


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


///{
/**
 *@define GS_SERVO_
 *specifies servo mappings i.e. physical layout of servos on the controller
 */
#define GS_SERVO_HEAD 0
#define GS_SERVO_TORSO 1
#define GS_SERVO_LEFTARM 2
#define GS_SERVO_RIGHTARM 3
///}

///{
/**
 *@define GS_STARTPOS_
 *default postions of servos
 */
#define GS_STARTPOS_HEAD 103
#define GS_STARTPOS_TORSO 110
#define GS_STARTPOS_RIGHTARM 200
#define GS_STARTPOS_LEFTARM 40
///}

///{
/**
 *@define GS_*EYE
 *LED mapping for the eyes
 */
#define GS_RIGHTEYE 0
#define GS_LEFTEYE 2
///}

///@{
/**
 *@name GS_SET*
 *@define GS_SET*
 *defined code to aid readability in long getsure functions.
 *a CPhidgetAdvancedServoHandle
 *b servo index
 *c position or state
 */

#define GS_SETPOS(a, b, c) if((error = CPhidgetAdvancedServo_setPosition \
                             (a, b, c)) != 0) { \
                             return error; \
                           }
#define GS_SETENG(a, b, c) if((error = CPhidgetAdvancedServo_setEngaged \
                             (a, b, c)) != 0) { \
                             return error; \
                           }
#define GS_SETACC(a, b, c) if((error = CPhidgetAdvancedServo_setAcceleration \
                             (a, b, c)) != 0) { \
                             return error; \
                           }
#define GS_SETVEL(a, b, c) if((error = CPhidgetAdvancedServo_setVelocityLimit \
                             (a, b, c)) != 0) { \
                             return error; \
                           }

///@}


///@{
/**
 *@name servo functions
 *These servo functions all act to create movements in the servos via directy
 * calls to the phidget library.
 *
 *@arg CPhidgetAdvancedServoHandle servo This is the servo handle.
 *
 *
 *@return int phidget library function call return code.
*/

/**
 *function should be called after every gesture to reset sevro positions to default.
 *Sets servo enaged status to 0 (off)
 */
int gs_set_pos(CPhidgetAdvancedServoHandle servo);
int gs_dance_basic(CPhidgetAdvancedServoHandle servo);
int gs_dance_advanced(CPhidgetAdvancedServoHandle servo);
int gs_raise_arms(CPhidgetAdvancedServoHandle servo);
int gs_shake_head(CPhidgetAdvancedServoHandle servo);
int gs_move_arms(CPhidgetAdvancedServoHandle servo);
int gs_turn(CPhidgetAdvancedServoHandle servo);
int gs_wave_left(CPhidgetAdvancedServoHandle servo);
int gs_wave_right(CPhidgetAdvancedServoHandle servo);
int gs_raise_right(CPhidgetAdvancedServoHandle servo);
int gs_raise_left(CPhidgetAdvancedServoHandle servo);

///@}


///@{
/**
 *@name LED functions
 *These functions control the phidget LEDs via interface kit calls to the 
 *phidget library.  Thse functions turn the eyes on and off. 
 *
 *@arg CPhidgetInterfaceKitHandle ifkit This is the interface kit handle.
 *
 *
 *@return int phidget library function call return code.
 */
int gs_eyeson(CPhidgetInterfaceKitHandle ifkit);
int gs_eyesoff(CPhidgetInterfaceKitHandle ifkit);
int gs_eyeflash(CPhidgetInterfaceKitHandle ifkit);
int gs_rapid_eyeflash(CPhidgetInterfaceKitHandle ifkit);

///@}

/**
 *This function allows playing of sound via an external program
 *
 *@arg const char *sound The sound name specified in the sound.json file.
 *@arg int repeat Allows the sound fucntion to repeat a sound multiple times.
 *@return value greater than zero if error.
 */
int gs_sound(const char *sound, int repeat);

/*
 * Takes a string as input, along with the LCD handle, and prints the string
 * to the LCD screen. Prints two lines at a time. Exits when the string has 
 * been printed.
 */
int gs_printstring(const char* str,CPhidgetTextLCDHandle txt_lcd);
#endif
