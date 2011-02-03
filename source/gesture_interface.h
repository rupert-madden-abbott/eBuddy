#ifndef GESTURE_INTERFACE_H
#define GESTURE_INTERFACE_H

#include <stddef.h>

#include <phidget21.h>

#include "gesture.h"
#include "phidget.h"

#define GSI_NULL 2
#define GSI_PHER 1
#define GSI_OK 0

int gsi_gesture_init(ph_handle *handle);
int gsi_gesture_close(ph_handle *handle);
int gsi_happy_level1(ph_handle *handle);
int gsi_fun_level1(ph_handle *handle);
int gsi_test(ph_handle *handle);
int gsi_shake_head(ph_handle *handle);
int gsi_move_arms(ph_handle *handle);
int gsi_raise_arms(ph_handle *handle);
int gsi_eyeflash(ph_handle *handle);
int gsi_sound(const char *sound, int repeat);

extern int gsi_printLCD(const char* str, ph_handle *handle);


typedef struct gsi_reaction {
    int (*gesture)(ph_handle *);	/* pointer to the gesture that should be called */
    const char *message;        /* string to display on the lcd screen */
    const char *sound;
    int sndrepeat;
	
} gsi_Reaction;

int gsi_react(const gsi_Reaction *resp, ph_handle *handle);
#endif
