#ifndef DEMO_H
#define DEMO_H


#include <stdio.h>
#include <unistd.h>

#include "emotion.h"
#include "input.h"
#include "gesture_interface.h"
#include "notify.h"
#include "queue.h"

int demo_main(em_State *emotions, qu_queue *notifications);

#endif
