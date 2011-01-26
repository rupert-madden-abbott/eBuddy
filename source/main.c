#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <phidget21.h>
#include <jansson.h>
#include <pthread.h>

#include "utility.h"
#include "queue.h"
#include "notify.h"
#include "phidget.h"
#include "gesture_interface.h"
#include "emotion.h"
#include "config.h"
#include "main.h"

int main(void) {
  em_State *emotions;
  error_code rc;
  
  /* initialise the phidgets */
  rc = ph_init(CONFIG_PATH);
  
  /* check for errors */
  if(rc) {
  	printf("Error initialising phidgits\n");
  	exit(1);
  }
  
  /* initialise notification system */
  //rc = nt_init(queue, notify_config);
  
  /* create a new emotion state using the emotion table */
  emotions = em_create(EMOTIONS, NUM_EMOTIONS);
  
  /* check for errors */
  if(!emotions) {
    printf("Error initialising emotions\n");
    printf("Check enough memory is available and try again\n");
  	exit(1);
  }
  
  /* load the emotion values from the last session */
  rc = em_load(emotions, EM_STATE_PATH);
  
  /* if file is corrupt keep running using defaults */
  if(rc == ERR_BAD_FILE) {
    printf("Error: state file is corrupt\n");
    printf("Reseting to defaults\n");
  }
  
  /* if file doesn't exist but path is valid keep running */
  else if(rc == ERR_BAD_PATH && test_path(EM_STATE_PATH)) {
    printf("Could not find state file\n");
    printf("Reseting to defaults\n");
  }
  
  /* all other errors are fatal */
  else {
    printf("Error reading state file\n");
    exit(1);
  }
  
  /* initialise the input library */
  //rc = ip_init(phidgets, input_config)

  //rc = run_mode(config, input_buffer, emotions, notifications, mode_interactive);
  

  
  /* finalise and unload all modules */
  em_save(emotions, EM_STATE_PATH);
  em_destroy(emotions);
  
  return 0;
}

#if 0
/* hooks to different modes go here */
int run_mode(const char *config, int *input_buffer, em_State *emotions, 
             nt_node message, int mode); {
	
  /* interactive (normal) mode */
  if(mode == mode_interactive) {
    return interactive_mode(config, input, emotions, notifications);
  }
  
  /* guessing game */
  else if(mode == mode_guess) {
    return game_run_guess(config, input, emotions, notifications);
  }
}

int interactive_mode(const char *config, int *input_buffer, em_State *emotions, 
                     nt_node message) {
  EmotionReaction *emotion_reaction;
  GestureReaction *gesture_reaction;
  em_condition condition;
  em_Event emotion_event; 
  int input_event;

  /* look for input events */
  rc = inp_check(input_buffer, input_event);
  
  /* react to events */
  if(!rc) {
  	
  	/* get action from table */
  	input_reaction = input_action[input_event]);
  	
  	/* get current condition */
  	condition = em_get_condition(state, input_reaction->primary_emotion->emotion);
  	
  	/* update emotions */
  	rc = em_react(input_reaction->primary_emotion);
  	
  	/* check for errors */
  	if(rc) {
  	  return err_bad_update;
  	}
  	
    rc = em_react(input_reaction->secondary_emotion);
    
    /* check for errors */
  	if(rc) {
  	  return err_bad_update;
  	}
    
    /* if emotion was full do full gesture */
    if(condition == em_cond_full) {
      ges_react(input_reaction->full_gesture);
    }
    
    /* if it was normal do normal gesture */
    else if(condition == em_cond_normal) {
      ges_react(input_reaction->normal_gesture);
    }
    
    /* otherwise do low gesture */
    else {
      ges_react(input_reaction->low_gesture);
    }
    
    /* if the input triggers a mode change switch to that mode */
    if(input_reaction->mode) {
      rc = do_mode(input_reaction->mode)
      
      /* return errors */
      if(rc) {
        return rc;
      }
    }
  }
  
  /* look for emotion events */
  rc = em_check(emotions, &emotion_event);
  
  /* react to events if there was one */
  if(!rc) {
  	
  	/* get the correct set of reations */
  	emotion_reaction = emotion_action[emotion_event.emotion];
  	
  	/* do full gesture if condition is full */
    if(emotion_event.type == em_cond_full) {
      ges_react(emotion_reaction->full_gesture);
    }
    
    /* do low gesture if condition is low */
    else if(emotion_event.type == em_cond_low) {
      ges_react(emotion_reaction->low_gesture);
    }
    
    /* do critical gesture if condition is critical */
    else {
      ges_react(emotion_reaction->low_gesture);
    }
  }
  
  /* get notification events */
  rc = not_check(notifications, gesture_reaction);
  
  /* run the reaction directly */
  if(rc) {
    ges_react(gesture_reaction);
  }
  
  sleep(1)
}
#endif
