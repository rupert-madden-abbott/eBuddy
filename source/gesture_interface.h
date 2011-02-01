#ifndef GESTURE_INTERFACE_H
#define GESTURE_INTERFACE_H

#include <stddef.h>

#include <phidget21.h>

#include "gesture.h"
#include "phidget.h"

#define GSI_NULL 2
#define GSI_PHER 1
#define GSI_OK 0

extern int gsi_happy_level1(Phhandle handle);
extern int gsi_fun_level1(Phhandle handle);
extern int gsi_notification(void);
extern int gsi_test(Phhandle handle);
extern int gsi_shake_head(Phhandle handle);
extern int gsi_move_arms(Phhandle handle);
extern int gsi_raise_arms(Phhandle handle);
extern int gsi_eyeflash(Phhandle handle);

extern int gsi_printLCD(const char* str, Phhandle handle);

/* representation of a gesture [needs to go in gesture] */
typedef struct gsi_reaction {
  int (*gesture)(Phhandle);	/* pointer to the gesture that should be called */
  const char *message;	/* string to display on the lcd screen */
} gsi_Reaction;

int gsi_react(const gsi_Reaction *resp, Phhandle handle);
#endif
