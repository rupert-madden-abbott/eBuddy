/**
 *@file gesture_interface.h
 *@author Andrew Barber
 *
 *This module acts to interface between main functions and the gesture module.
 *Funtions in this module take a ph_handle struct as an argument and return an
 *error.
 *This module allows the mapping of multiple gestures to a single interface
 *call.
 *This module also contains initialisation for the phidget output.
 */


#ifndef GESTURE_INTERFACE_H
#define GESTURE_INTERFACE_H

#include <stddef.h>

#include <phidget21.h>

#include "gesture.h"
#include "phidget.h"

/**
 *@enum GSI_*error
 *gsi_ error codes
 */
typedef enum gsi_error {
 GSI_OK = 0,
 GSI_PHER =  1,
 GSI_NULL = 2
} gsi_error;

/**
 *@struct used by gsi_react function to determine function to call and pass correct value.  
*/

typedef struct gsi_reaction {
  int (*gesture)(ph_handle *);	//pointer to the gesture that should be called. 
  const char *message;          //string to display on the lcd screen.
  const char *sound;            //Sound to be played.
  int sndrepeat;                //Sound repeat.
	
} gsi_Reaction;


///@{
/**
 *@name gesture interface functions
 *These are standard gsi_ functions that map to one or more gestures.
 *
 *@arg ph_handle *handle Struct points to the handles for each phidget.
 *@return GSI error code.
 */
int gsi_gesture_init(ph_handle *handle);
int gsi_gesture_close(ph_handle *handle);
int gsi_happy_level1(ph_handle *handle);
int gsi_fun_level1(ph_handle *handle);
int gsi_test(ph_handle *handle);
int gsi_shake_head(ph_handle *handle);
int gsi_move_arms(ph_handle *handle);
int gsi_raise_arms(ph_handle *handle);
int gsi_eyeflash(ph_handle *handle);
int gsi_rapid_eyeflash(ph_handle *handle);
int gsi_raise_left(ph_handle *handle);
int gsi_raise_right(ph_handle *handle);
int gsi_wave_left(ph_handle *handle);
int gsi_wave_right(ph_handle *handle);
int gsi_turn(ph_handle *handle);

///@}

/**
 *This function allows playing of sound via an external program
 *
 *@arg const char *sound The sound name specified in the sound.json file.
 *@arg int repeat Allows the sound function to repeat a sound multiple times.
 *@return value greater than zero if error.
 */
int gsi_sound(const char *sound, int repeat);

/** 
 *This fucntion facilitates printing on the LCD screen.
 *
 *@arg const char* str String to be printed on the screen.
 *@arg ph_handle *handle Struct points to the handles for each phidget.
 *@return error code.
 */ 
int gsi_printLCD(const char* str, ph_handle *handle);

/**
 *Function handles calling of gsi_ functions incuding lcd text, sounds and
 *gestures. 
 *
 *@arg const gsi_Reaction *resp Pointer containing fcuntions with arguments.
 *@arg ph_handle *handle Struct points to the handles for each phidget.
 *@return GSI error code.
*/

int gsi_react(const gsi_Reaction *resp, ph_handle *handle);
#endif
