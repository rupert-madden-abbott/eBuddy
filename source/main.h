#ifndef MAIN_H
#define MAIN_H

#include "emotion.h"

//Number of emotions in the emotion table
#define NUM_EMOTIONS 5

//path to the configuration directory
#define CONFIG_PATH "conf/"

//notification configuration file
#define NT_CONFIG CONFIG_PATH "notify.json"

//emotion state file
#define EM_STATE_PATH CONFIG_PATH "em_state"

//the mode to run on startup
#define STARTUP_MODE MODE_REACT

//list of emotions and their tabe row numbers
typedef enum emotion_type {
  EMO_NONE = -1,
  EMO_HUNGER = 0,
  EMO_ENERGY = 1,
  EMO_CLEAN = 2,
  EMO_SOCIAL = 3,
  EMO_FUN = 4
} emotion_type;

//list of emotions and their decay times etc
//each emotion will decay to 1*factor of its original value
//after one life. when the level is above full or below low
//the user will be alerted once every alert time. all times
//are in seconds.
const em_Emotion main_emotions[NUM_EMOTIONS];

#endif
