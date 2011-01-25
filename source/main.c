//#include "main.h"
/*#include "notify.h"
#include "phidget.h"
#include "gesture_interface.h"
#include "emotion.h"
#include "config.h"*/

int main(void) {
  #if 0
  const char    *phidget_config = NULL, *notify_config = NULL, 
                *gesture_config = NULL, *input_config = NULL, 
                *emotion_config = NULL; 
  cf_json       *root = NULL;
  nt_node       *queue = NULL;
  PhidgetHandle *phidgets;
  int           *input_buffer, rc;
  
  /* load config */
  root = cf_read(CONFIG_PATH);
  if(!root) return err_unknown;
  
  phidget_config = cf_get_string(root, "phidget_config");
  notify_config = cf_get_string(root, "notify_config");
  gesture_config = cf_get_string(root, "gesture_config");
  input_config = cf_get_string(root, "input_config");
  emotion_config = cf_get_string(root, "emotion_config");
  cf_free(root);
  if(!phidget_config || !notify_config || !gesture_config) return err_unknown;
  
  /* initialise the phidgets */
  rc = ph_init(phidgets, phidget_config);
  if(rc) return err_unknown;
  
  /* initialise the gesture library */
  rc = gs_init(phidgets, gesture_config);
  
  /* check for errors */
  if(rc) {
  }
  
  rc = nt_init(queue, notify_config);
  
  /* check for errors */
  if(rc) {
  }
  
  rc = em_init(emotion_config);
  
  /* check for errors */
  if(rc) {
  }
  
  /* initialise the input library */
  //rc = ip_init(phidgets, input_config)

  rc = run_mode(config, input_buffer, emotions, notifications, mode_interactive);
  
  /* check for errors */
  if(rc) {
  }
  
  /* finalise and unload all modules */
  #endif
  
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
