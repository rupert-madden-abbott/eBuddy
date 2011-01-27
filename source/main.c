#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <phidget21.h>
#include <jansson.h>
#include <pthread.h>

#include "utility.h"
#include "phidget.h"
#include "input.h"
#include "emotion.h"
#include "gesture_interface.h"
#include "config.h"
#include "queue.h"
#include "notify.h"
#include "main.h"

int main(void) {
  em_State *emotions;
  qu_queue *notifications;
  error_code rc;

  //create a new emotion state using the emotion table
  emotions = em_create(EMOTIONS, NUM_EMOTIONS);

  if(!emotions) {
    printf("Error initialising emotions\n");
    printf("Check enough memory is available and try again\n");
  	exit(1);
  }
  
  //load the emotion values from the last session
  rc = em_load(emotions, EM_STATE_PATH);
  
  //if file is corrupt keep running using defaults
  if(rc == ERR_BAD_FILE) {
    printf("Error: state file is corrupt\n");
    printf("Reseting to defaults\n");
  }
  
  //if file doesn't exist but path is valid keep running
  else if(rc == ERR_BAD_PATH && test_path(EM_STATE_PATH)) {
    printf("Could not find state file\n");
    printf("Reseting to defaults\n");
  }
  
  //all other errors are fatal
  else if(rc) {
    printf("Error reading state file\n");
    exit(1);
  }
  
  //initialise the phidgets
  rc = ph_init(CONFIG_PATH);
  if(rc) {
  	printf("Error initialising phidgits\n");
  	exit(1);
  }
  
  //create the notification queue
  notifications = qu_init();
  
  if(!notifications) {
    printf("Could not initialise notification queue\n");
    printf("Check enough memory is available and try again\n");
    exit(1);
  }
  
  //initialise notification system
  rc = nt_init(notifications, NT_CONFIG);
  
  if(rc) {
  	printf("Error initialising notification system\n");
  	exit(1);
  }
  
  //enter main interactive mode
  rc = enter_mode(MODE_MAIN, emotions, notifications);
  
  //finalise and unload all modules
  printf("Shutting down\n");
  em_save(emotions, EM_STATE_PATH);
  em_destroy(emotions);
  //nt_destroy();
  ph_destruct();
  return 0;
}


//takes a run_mode and calls the appropriate function
int enter_mode(run_mode mode, em_State *emotions, qu_queue *notifications) {
	
  /* main mode */
  if(mode == MODE_MAIN) {
    return run_main(emotions, notifications);
  }
  
  /* demo moode */
  else if(mode == MODE_DEMO) {
    return run_demo(emotions, notifications);
  }
  
  /* guessing game */
  else if(mode == MODE_GUESS) {
    //return game_run_guess(emotions, notifications);
  }
  
  //check for incorrect mode number
  return ERR_BAD_MODE;
}



//Demo mode
int run_demo(em_State *emotions, qu_queue *notifications) {
  nt_message *message;

  while(1) {
    sleep(1);
    if(qu_size(notifications) > 0) {
      printf("Before POP: %i\n", qu_size(notifications));
      message = qu_pop(notifications);
      printf("After POP: %i\n", qu_size(notifications));
      printf("%s\n", message->text);
      gsi_eyeflash();
      gsi_raise_arms();
      gsi_notification();
      gsi_printLCD(message->text);
      
      fflush(stdout);
    }
      if(in_get_input() == INPT_BATTERY)
	{
            
		gsi_happy_level1();
	}
  }


  return 1;
}


//Main loop
int run_main(em_State *emotions, qu_queue *notifications) {
  const EmotionAction *em_action;
  const InputAction *in_action;
  nt_message *message;
  em_Event emotion_event; 
  em_condition condition;
  int input_event, rc;

  while(1) {

  //look for input events
  input_event = in_get_input();
  
  //react to events
  if(input_event) {
  	
  	//get action from table
  	in_action = &input_actions[input_event - 1];
  	
  	printf("got input %d\n", input_event);
  	
  	//get condition of primary emotion before update
  	condition = em_get_condition(emotions, in_action->primary_emotion.emotion);
  	
  	//update primary emotion
  	rc = em_react(emotions, &in_action->primary_emotion);
  	
  	printf("got rc %d\n", rc);
  	
  	//check for errors in reaction table
  	assert(rc ==0);
  	
  	//update secondary emotion
    rc = em_react(emotions, &in_action->secondary_emotion);
    
    printf("got rc %d\n", rc);
    
  	//check for errors in reaction table
  	assert(rc ==0);
    
    //if emotion was full do full gesture
    if(condition == EM_COND_FULL) {
      gsi_react(&in_action->full_gesture);
    }
    
    //if it was normal do normal gesture
    else if(condition == EM_COND_NORMAL) {
      gsi_react(&in_action->normal_gesture);
    }
    
    //otherwise do low gesture
    else {
      gsi_react(&in_action->low_gesture);
    }
    
    //if the input triggers a mode change switch to that mode
    if(in_action->mode) {
      rc = enter_mode(in_action->mode, emotions, notifications);
      
      //return errors to parent mode
      if(rc) {
        return rc;
      }
    }
  }
  
  //look for emotion events
  rc = em_check(emotions, &emotion_event);
  
  /* react to events if there was one */
  if(!rc) {
  	
  	/* get the correct set of reations */
  	em_action = &emotion_actions[emotion_event.emotion];
  	
  	/* do full gesture if condition is full */
    if(emotion_event.type == EM_COND_FULL) {
      gsi_react(&em_action->full_gesture);
    }
    
    /* do low gesture if condition is low */
    else if(emotion_event.type == EM_COND_LOW) {
      gsi_react(&em_action->low_gesture);
    }
    
    /* do critical gesture if condition is critical */
    else {
      gsi_react(&em_action->low_gesture);
    }
  }
  
  /* get notification events */
  message = qu_pop(notifications);
  
  /* run the reaction directly */
  if(message) {
    gsi_react(&message_action);
  }
  
  sleep(1);
}
}

