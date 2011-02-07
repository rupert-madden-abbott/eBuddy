/** 
 * @file main.c
 * @author Rowan Saundry
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"
#include "phidget.h"
#include "input.h"
#include "emotion.h"
#include "queue.h"
#include "notify.h"
#include "gesture_interface.h"
#include "react.h"
#include "guess.h"
#include "debug.h"
#include "demo.h"

#include "main.h"

const em_Emotion mn_emotions[] = {
  
//name        life           factor   alert         max    full  low  critical
  {"hunger",  UT_HOUR * 24,  0.2,     UT_MIN * 5,   100,   95,   30,  10},
  {"energy",  UT_HOUR * 18,  0.5,     UT_MIN * 13,  100,   95,   20,  10},
  {"clean",   UT_HOUR * 24,  0.3,     UT_MIN * 7,   150,   100,  50,  10},
  {"social",  UT_HOUR * 2,   0.9,     UT_MIN * 9,   100,   98,   40,  20},
  {"fun",     UT_HOUR * 2,   0.7,     UT_MIN * 11,  100,   98,   20,  5}
};


int main(void) {
  em_State *emotions;
  qu_queue *notifications;
  ph_handle phhandle;
  ut_ErrorCode rc;

  //create a new emotion state using the emotion table
  emotions = em_init(mn_emotions, MN_NUM_EMOTIONS);

  if(!emotions) {
    printf("Error initialising emotions\n");
    printf("Check enough memory is available and try again\n");
    exit(1);
  }

  //load the emotion values from the last session
  rc = em_load(emotions, MN_EM_STATE_PATH);
   /* 
  //if file is corrupt keep running using defaults
  if(rc == UT_ERR_BAD_FILE) {
    printf("Error: state file is corrupt\n");
    printf("Reseting to defaults\n");
  }
  
  //if file doesn't exist but path is valid keep running
  else if(rc == UT_ERR_BAD_PATH && ut_test_path(MN_EM_STATE_PATH)) {
    printf("Could not find state file\n");
    printf("Reseting to defaults\n");
  }
  
  //all other errors are fatal
  else if(rc) {
    printf("Error reading state file\n");
    em_destroy(emotions);
    exit(1);
  }
  
  //initialise the phidgets
  rc = ph_init(MN_CONFIG_PATH, &phhandle);
  
  if(rc) {
    printf("Error initialising phidgits\n");
    em_destroy(emotions);
    exit(1);
  }

  //initialise gestures
  rc = gsi_gesture_init(&phhandle);
  
  if(rc) {
    printf("Error initialising gestures\n");
    em_destroy(emotions);
    ph_destruct(&phhandle);
    exit(1);
  }

  //initialise input 
  rc = in_input_init(&phhandle);
  
  if(rc) {
    printf("Error initialising input\n");
    gsi_gesture_close(&phhandle);
    em_destroy(emotions);
    ph_destruct(&phhandle);
    exit(1);
  }*/
  
  //create the notification queue
  notifications = qu_init();
  
  if(!notifications) {
    printf("Could not initialise notification queue\n");
    printf("Check enough memory is available and try again\n");
    gsi_gesture_close(&phhandle);
    em_destroy(emotions);
    ph_destruct(&phhandle);
    exit(1);
  }
  
  //initialise notification system
  rc = nt_init(notifications, NT_CONFIG);
  
  if(rc) {
    printf("Error initialising notification system\n");
    exit(1);
  }
  
  //enter main interactive mode
  rc = mn_run(MN_STARTUP_MODE, emotions, notifications, &phhandle);
  
  //finalise and unload all modules
  printf("Shutting down\n");
  em_save(emotions, MN_EM_STATE_PATH);
  em_destroy(emotions);
  gsi_gesture_close(&phhandle);
  nt_destroy(notifications);
  ph_destruct(&phhandle);
  return 0;
}

ut_ErrorCode mn_run(mn_mode mode, em_State *emotions, qu_queue *notifications, ph_handle *phhandle) {
  
  switch(mode) {
    case MN_REACT:
      return rc_main(emotions, notifications, phhandle);
      
    case MN_SLEEP:
      return rc_sleep(emotions, notifications, phhandle);
      
    case MN_DEMO:
      return dm_main(emotions, notifications, phhandle);
      
    case MN_DEBUG:
      return db_main(emotions, notifications, phhandle);

    case MN_GUESS:
      return guess_main(emotions, notifications, phhandle);

    case MN_NONE:
      return UT_ERR_NONE;
      
    default:
      return UT_ERR_BAD_MODE;
  } 
}

