#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include "utility.h"
#include "emotion.h"

/* allocate and return a pointer to a new emotional state */
em_State *em_create(const em_Emotion *emotions, int num_emotions) {
  em_State *state;
  time_t now;
  int i;
  
  /* get current time */
  now = time(NULL);
	
  /* allocate memory for state */
  state = (em_State *) malloc(sizeof(em_State));
	
  /* return NULL on error */
  if(!state) {
    return NULL;
  }
  
  state->num_emotions = num_emotions;
  state->emotions = emotions;

  /* allocate one level for each emotion */
  state->levels = (em_Level *) malloc(num_emotions * sizeof(em_Level));
  	
  /* check alloc */
  if(!state->levels) {
    free(state);
    return NULL;
  }
	
  /* initialise each level */
  for(i = 0; i < num_emotions; i++) {
    state->levels[i].last_value = 0;
	state->levels[i].last_update = now;
	state->levels[i].last_event = now;
  }
	
  return state;
}

/* free up a state */
void em_destroy(em_State *state) {
  free(state->levels);	
  free(state);
}

/* return all levels to their default values (0) */
void em_reset(em_State *state) {
  time_t now;
  int i;
	
  now = time(NULL);
	
  for(i = 0; i < state->num_emotions; i++) {
    state->levels[i].last_value = 0;
    state->levels[i].last_update = now;
    state->levels[i].last_event = now;
  }
}

/* load an emotional state from a file */
int em_load(em_State *state, const char *path) {
  FILE *file;
  struct tm timestamp;
  time_t now;
  int i, rc;

  /* get current time */
  now = time(NULL);

  /* open the file */
  file = fopen(path, "r");
	
  /* check file opened correctly */
  if(!file) {
    return ERR_BAD_PATH;
  }
	
  /* read the value and timestamp of each level from the file */
  for(i = 0; i < state->num_emotions; i++) {
    rc = fscanf(file, "%d/%d/%d %d:%d:%d  %lf\n", &timestamp.tm_mday, &timestamp.tm_mon,
                  &timestamp.tm_year, &timestamp.tm_hour, &timestamp.tm_min, &timestamp.tm_sec, &state->levels[i].last_value);
    
    /* return an error if items diddn't scan */
    if(rc != 7) {
      return ERR_BAD_FILE;
    }
    
    /* convert calendar time to a time_t */
    timestamp.tm_isdst = -1;
    timestamp.tm_year -= 1900;
    timestamp.tm_mon--;
    
    state->levels[i].last_update = mktime(&timestamp);
    
    /* set the last event time to now */
    state->levels[i].last_event = now;
  }
	
  fclose(file);
  return 0;
} 

/* save state to a file */
int em_save(em_State *state, const char *path) {
  struct tm *time;
  FILE *file;
  int i;
	
  /* open the file */
  file = fopen(path, "w");

  /* check file is open */
  if(!file) {
    return ERR_BAD_PATH;
  }

  /* write time and value of each level */
  for(i = 0; i < state->num_emotions; i++) {
  	
  	/* convert the timestamp into calander time */
  	time = gmtime(&state->levels[i].last_update);
  	
  	/* print the value and the date to the file */
    fprintf(file, "%02d/%02d/%02d %02d:%02d:%02d  %lf\n", time->tm_mday, time->tm_mon + 1,
              time->tm_year + 1900, time->tm_hour, time->tm_min, time->tm_sec, state->levels[i].last_value);
  }
  
  /* close and return */
  fclose(file);
  return 0;
}

/* returns the level of an emotion */
double em_get(em_State *state, int emotion) {
  double difference, num_decays, value;
  
  /* calculate time since last update */
  difference = difftime(time(NULL), state->levels[emotion].last_update);
  
  /* work out the number of decay times that have passed */
  num_decays = difference / (double) SECONDS_IN_AN_HOUR * (double) state->emotions[emotion].decay_time;
  
  /* caluclate the current value */
  value = state->levels[emotion].last_value  * pow(state->emotions[emotion].factor, num_decays);
  
  return value;
}

/* return the condition of an emotion */
em_condition em_get_condition(em_State *state, int emotion) {
int level;

level = em_get(state, emotion);

if(level > state->emotions[emotion].full) {
  return em_cond_full;
}

else if(level < state->emotions[emotion].critical) {
  return em_cond_critical;
}

else if(level < state->emotions[emotion].low) {
  return em_cond_low;
}

else {
  return em_cond_normal;
}
}


/* returns the average value of all the levels in a state */
double em_overall(em_State *state) {
  double total;
  int i;

  total = 0;
  	
  for(i = 0; i < state->num_emotions; i++) {
    total += em_get(state, i);
  }

  return total / (double) state->num_emotions;
}

/* sets the level of an emotion to the value given. will not allow the
 * level to go above EM_MAX_LEVEL or below 0. returns the condition of
 * the emotion after the update */
int em_set(em_State *state, int emotion, double value) {

  /* check bounds */
  if(value > EM_MAX_LEVEL || value < 0) {
    return ERR_BAD_ARG;
  }
	
  state->levels[emotion].last_value = value;
  state->levels[emotion].last_update = time(NULL);
  
  return 0;
}

/* adds the value given to the level of an emotion. will not allow the
 * level to go above EM_MAX_LEVEL or below 0. */
int em_update(em_State *state, int emotion, double value) {
  double total;
	
  /* add the previous value to the update vale to get the total */
  total = value + em_get(state, emotion);
	
  /* if the total is above the maximum level then set the level to maximum */
  if (total > EM_MAX_LEVEL) {
    em_set(state, emotion, EM_MAX_LEVEL);
  }
  
  /* if the total is smaller than zero set the level to zero */
  else if(total < 0) {
    em_set(state, emotion, 0);
  }

  /* otherwise set the level to the total */
  else {
    em_set(state, emotion, total);	
  }
  
  return 0;
}

/* checks for emotional events and stores the first one found in event.
 * returns 0 if an event was found or 1 otherwise */
int em_check(em_State *state, em_Event *event) {
  em_condition condition;
  time_t now;
  int i;

  /* get current time */
  now = time(NULL);

  /* loop over all the levels */
  for(i = 0; i < state->num_emotions; i++) {
  	
    /* check if the emotion is due for an event */
    if(state->emotions[i].event_time < difftime(now, state->levels[i].last_event)) {
    
      /* get the condition of the emotion */
      condition = em_get_condition(state, i);

      /* */ 
  	  if(condition) {
  	    /* update last event */
  	    state->levels[i].last_event = now;
  	  	
  	  	/* write the event struct */
  	  	event->type = condition;
  	  	event->emotion = i;
  	  	
  	    return 0;
  	  }
    } 
  }
  
  /* return no event */
  return 1;
}

/* updates a state according to the a reaction struct. */
int em_react(em_State *state, em_Reaction *reaction) {
  
  /* set the emotion to the value if action is set */
  if(reaction->action == em_action_set) {
  	return em_set(state, reaction->emotion, reaction->value);
  }
  
  /* update it if action is update */
  else if(reaction->action == em_action_update) {
    return em_update(state, reaction->emotion, reaction->value);
  }
  
  /* otherwise return an error */
  else {
    return 1;
  }
}
