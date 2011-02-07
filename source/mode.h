/*
  list of different run modes
  
  There is a circular dependency between react and mode and enums can't be
  forward declared. Therefore, this must be defined before react is included.
*/

#ifndef MODE_H
#define MODE_H

#include "emotion.h"
#include "phidget.h"
#include "queue.h"

typedef enum md_mode {
  MODE_END = -1,
  MODE_NONE = 0,
  MODE_REACT = 1,
  MODE_SLEEP = 2,
  MODE_DEMO = 3,
  MODE_DEBUG = 4,
  MODE_GUESS = 5,
} md_mode;

int md_run(md_mode mode, em_State *emotions, qu_queue *notifications, ph_handle *phhandle);

#endif
