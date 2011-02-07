#ifndef MAIN_H
#define MAIN_H

#include "emotion.h"
#include "phidget.h"
#include "queue.h"

//Number of emotions in the emotion table
#define MN_NUM_EMOTIONS 5

//The path to the configuration directory
#define MN_CONFIG_PATH "conf/"

//Notification configuration file
#define MN_NT_CONFIG MN_CONFIG_PATH "notify.json"

//Emotion state file
#define MN_EM_STATE_PATH MN_CONFIG_PATH "em_state"

//The mode to run on startup
#define MN_STARTUP_MODE MN_REACT

//List of emotions and their array element numbers
typedef enum emotion_type {
  EMO_NONE = -1,
  EMO_HUNGER = 0,
  EMO_ENERGY = 1,
  EMO_CLEAN = 2,
  EMO_SOCIAL = 3,
  EMO_FUN = 4
} emotion_type;

typedef enum mn_mode {
  MN_END = -1,
  MN_NONE = 0,
  MN_REACT = 1,
  MN_SLEEP = 2,
  MN_DEMO = 3,
  MN_DEBUG = 4,
  MN_GUESS = 5,
} mn_mode;

/* Contains the emotion definition array which will be passed
 * to the emotion module.
 */
const em_Emotion mn_emotions[MN_NUM_EMOTIONS];

/**
 * Initialisises all modules and switches to the mode defined in
 * MN_STARTUP_MODE.
 */
int main(void);
int mn_run(mn_mode mode, em_State *emotions, qu_queue *notifications, ph_handle *phhandle);
#endif
