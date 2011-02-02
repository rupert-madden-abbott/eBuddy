#ifndef MODE_H
#define MODE_H

#include "emotion.h"
#include "queue.h"

//list of different run modes
typedef enum run_mode {
  MODE_END = -1,
  MODE_NONE = 0,
  MODE_REACT = 1,
  MODE_SLEEP = 2,
  MODE_DEMO = 3,
  MODE_DEBUG = 4,
  MODE_GUESS = 5,
} run_mode;


int mode_run(run_mode mode, em_State *emotions, qu_queue *notifications);

#endif