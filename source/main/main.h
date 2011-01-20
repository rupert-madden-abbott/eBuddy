#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <jansson.h>

#define DEBUG 1

#define ONE_MIN (60)
#define ONE_HOUR (60*60)
#define ONE_DAY (24*60*60)

#ifdef __linux__
#define OS LINUX
#endif
#ifndef NeXTBSD
#ifdef __APPLE__
#define OS OSX
#endif
#endif

/* representation of a gesture [needs to go in gesture] */
typedef struct ges_response {
  void *gesture;	/* pointer to the gesture that should be called */
  char *message;	/* string to display on the lcd screen */
} ges_Response;

typedef enum error_code {
  err_unknown = -1,	/* unknow error */ 
  err_none = 0,		/* no error */
  err_bad_path,		/* file path is invalid */
  err_bad_file		/* file is corrupt */
} error_code;

/* represents the action to take when an input is recieved */
typedef struct input_action {
  em_Response primary_emotion;
  em_Response secondary_emotion;
  ges_Response full_gesture;
  ges_Response normal_gesture;
  ges_Response low_gesture;
  int mode;
} InputAction;

/* represents the gesture to perform when an emotion reaches a certain level */
typedef struct emotion_action {
  ges_Response full_gesture;
  ges_Response low_gesture;
  ges_Response critical_gesture;
  int mode;
} EmotionAction;

/* list of emotions and their decay times etc */
const em_Emotion emotions = {
  {24,	0.5,	10,	800,	200,	100},	/* hungar */
  {}
};

/* list of reactions for each emoton */
const EmotionAction emotion_actions[] = {
  {{NULL,	""}, {NULL,	""}, {NULL,	""},	0}		/* hunger */
}

const InputAction input_actions[] = {
  {{0,	0,	0},	{0,	0,	0}, {NULL,	""}, {NULL,	""}, {NULL,	""},	0}	/* somthing happens */

}


enum os { LINUX, OSX, WINDOWS };

void end(void);
int main_loop(void);

#endif
