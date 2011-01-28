#ifndef MODE_H
#define MODE_H

#include "emotion.h"
#include "queue.h"

//list of different run modes
typedef enum run_mode {
  MODE_KILL = -1,
  MODE_NONE = 0,
  MODE_REACT,
  MODE_SLEEP,
  MODE_DEMO,
  MODE_DEBUG,
  MODE_GUESS,
} run_mode;


int mode_run(run_mode mode, em_State *emotions, qu_queue *notifications);

#endif