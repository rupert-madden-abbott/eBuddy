/**
 * The react module reads events from the emotion, notification and input
 * modules and controls how the eBuddy reacts to them. It contains two main
 * loops, one for normal interaction and one for sleeping. The eBuddys
 * response to each event is controlled by a set of statically defined arrays
 * and structs. 
 */

#ifndef REACT_H
#define REACT_H

#include "utility.h"
#include "main.h"
#include "emotion.h"
#include "gesture_interface.h"
#include "input.h"

/**
 * Represents the action to take when an input is recieved or when sleeping.
 * The full gesture will be performed if the level of the primary emotion is
 * full before the emotions are updated. The normal gesture will be used if
 * the emotion is not full low or critical. The low gesture is used if the
 * emotion is either low or critical. Mode contains the mode to execute after
 * the gesture if the level is critical in the main loop or if it is full in
 * the sleep loop.
 */
typedef struct rc_input_action {
  const em_Reaction primary_emotion;
  const em_Reaction secondary_emotion;
  const gsi_Reaction full_gesture;
  const gsi_Reaction normal_gesture;
  const gsi_Reaction low_gesture;
  const mn_mode mode;
} rc_InputAction;

/**
 * Represents the action to perform when an emotion reaches a certain level. 
 * There is one gesture for each event type. Critical mode contains the mode
 * to execute after the gesture if the emotion level is critical.
 */
typedef struct rc_emotion_action {
  const gsi_Reaction full_gesture;
  const gsi_Reaction low_gesture;
  const gsi_Reaction critical_gesture;
  const mn_mode critical_mode;
} rc_EmotionAction;


/**
 * The emotion action array contains one emotion action for each emotion
 * and is indexed by emotion number 
 */
const rc_EmotionAction RC_EM_ACTION[MN_NUM_EMOTIONS];


/* The input action array contains one input action for each input and
 * is indexed by input number. It is 1 based not zero based so for example
 * element 3 refers to input number 4.
 */
const rc_InputAction RC_IN_ACTION[IN_NUM_INPUTS];


/**
 * The msg action struct contains the gesture to perform when a message arrives
 * before displaying it on the screen.
 */
const gsi_Reaction RC_MSG_ACTION;

//The time to pause in seconds after each loop
const int RC_MAIN_PAUSE;
const int RC_SLEEP_PAUSE;


/**
 * This function contains the eBuddys main interaction loop. It reacts to input,
 * notification and emotion events by looking up a response in the action arrays
 * and executing it.
 *
 * @arg emotions The eBuddys emotion state
 * @arg notifications The notification queue
 * @arg phhandle The handle used to access the phidgets
 * @return A ut_error_code. Passes back errors from child modes.
 */
ut_ErrorCode rc_main(em_State *emotions, qu_queue *notifications, ph_handle *phhandle);

/**
 * The sleep loop is similar to the main loop and also reads events from the input
 * and notification modules but it does not map them to gestures. Instead it continuously
 * performs the same action and silently prints notifications on the screen. The action
 * is stored in the rc_sleep_action struct and is of the same type as the input actions.
 *
 * @arg emotions The eBuddys emotion state
 * @arg notifications The notification queue
 * @arg phhandle The handle used to access the phidgets
 * @return A ut_error_code. Passes back errors from child modes.
 */
ut_ErrorCode rc_sleep(em_State *emotions, qu_queue *notifications, ph_handle *phhandle);

#endif
