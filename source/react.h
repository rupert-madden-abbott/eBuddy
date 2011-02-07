#ifndef REACT_H
#define REACT_H

#include <stddef.h>
#include <unistd.h>
#include <assert.h>

#include "utility.h"
#include "emotion.h"
#include "phidget.h"
#include "gesture_interface.h"
#include "input.h"
#include "mode.h"
#include "notify.h"
#include "queue.h"
#include "main.h"

//represents the action to take when an input is recieved
typedef struct input_action {
  const em_Reaction primary_emotion;
  const em_Reaction secondary_emotion;
  const gsi_Reaction full_gesture;
  const gsi_Reaction normal_gesture;
  const gsi_Reaction low_gesture;
  const md_mode mode;
} InputAction;

//represents the gesture to perform when an emotion reaches a certain level
typedef struct emotion_action {
  const gsi_Reaction full_gesture;
  const gsi_Reaction low_gesture;
  const gsi_Reaction critical_gesture;
  const md_mode critical_mode;
} EmotionAction;


//list of alerts for each emotion
//each reaction is a gesture function and a text string for the lcd
const EmotionAction rc_em_action[NUM_EMOTIONS];


//list of alerts and updates for each input
const InputAction rc_in_action[IN_NUM_INPUTS];


//reaction to notifications
const gsi_Reaction rc_msg_action;

//main loop
int rc_main(em_State *emotions, qu_queue *notifications, ph_handle *phhandle);
int rc_sleep(em_State *emotions, qu_queue *notifications, ph_handle *phhandle);

#endif
