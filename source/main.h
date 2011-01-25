#ifndef MAIN_H
#define MAIN_H

/*#include "emotion.h"
#include "gesture_interface.h"
#include "notify.h"*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jansson.h>
#include "notify.h"
#include "phidget.h"
#include "gesture_interface.h"
#include "emotion.h"
#include "config.h"*/

#define CONFIG_PATH "conf/main.json"

#define ONE_MIN (60)
#define ONE_HOUR (60*60)
#define ONE_DAY (24*60*60)

/* represents the action to take when an input is recieved */
typedef struct input_action {
  em_Response primary_emotion;
  em_Response secondary_emotion;
  gsi_Response full_gesture;
  gsi_Response normal_gesture;
  gsi_Response low_gesture;
  int mode;
} InputAction;

/* represents the gesture to perform when an emotion reaches a certain level */
typedef struct emotion_action {
  gsi_Response full_gesture;
  gsi_Response low_gesture;
  gsi_Response critical_gesture;
  int mode;
} EmotionAction;

/**
 * \todo Placeholder for PhidgetHandle needs to be filled out
 */
typedef struct PhidgetHandle {
  void *example;
} PhidgetHandle;

/**
 * \todo Currently sends out several warnings
 *  - warning: braces around scalar initializer
 *  - warning: excess elements in scalar initializer
 */
/* list of emotions and their decay times etc */
#if 0
const em_Emotion emotions = {
  {24,	0.5,	10,	800,	200,	100},	/* hungar */
  {}
};


/* list of reactions for each emoton */
const EmotionAction emotion_actions[] = {
  {{NULL,	""}, {NULL,	""}, {NULL,	""},	0}		/* hunger */
};

const InputAction input_actions[] = {
  {{0,	0,	0},	{0,	0,	0}, {NULL,	""}, {NULL,	""}, {NULL,	""},	0}	/* somthing happens */

};
#endif

void end(void);
int main_loop(void);
int run_mode(const char *config, int *input_buffer, em_State *emotions, nt_node message, int mode);
#endif
