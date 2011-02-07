#include "emotion.h"

/* allocate and return a pointer to a new emotional state */
em_State *em_create(const em_Emotion *emotions, int num_emotions) {
  em_State *state;
  time_t now;
  
  assert(emotions);
  assert(num_emotions);
  
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
	
  //set state to default values
  em_reset(state);
	
  return state;
}

/* free up a state */
void em_destroy(em_State *state) {
  assert(state);
  
  free(state->levels);	
  free(state);
}

/* return all levels to their default values (0) */
void em_reset(em_State *state) {
  time_t now;
  int i;
  
  assert(state);
	
  now = time(NULL);
	
  //initialise each level to full level
  for(i = 0; i < state->num_emotions; i++) {
    state->levels[i].last_value = state->emotions[i].full;
    state->levels[i].last_update = now;
    state->levels[i].last_event = now;
  }
}

/* Load a state from a file specified by path. The the number of emtions and
 * their names inside the file must match those in the array passed to init.
 * Returns UT_ERR_NONE on success, UT_ERR_BAD_PATH if the file cannot be read or,
 * UT_ERR_BAD_FILE if the file is not in the correct format.  */
int em_load(em_State *state, const char *path) {
  char name[EM_NAME_LEN];
  FILE *file;
  struct tm timestamp;
  time_t now;
  double value;
  int id, rc;
  
  assert(state);

  /* get current time */
  now = time(NULL);

  /* open the file */
  file = fopen(path, "r");
	
  /* check file opened correctly */
  if(!file) {
    return UT_ERR_BAD_PATH;
  }
	
  /* read the value and timestamp of each level from the file */
  do {
    rc = fscanf(file, "%10s %d/%d/%d %d:%d:%d  %lf\n", name, &timestamp.tm_mday, &timestamp.tm_mon,
                  &timestamp.tm_year, &timestamp.tm_hour, &timestamp.tm_min, &timestamp.tm_sec, &value);
    
    /* return an error if items diddn't scan */
    if(rc != EM_LINE_ITEMS) {
      return UT_ERR_BAD_FILE;
    }
    
    /* convert calendar time to a time_t */
    timestamp.tm_isdst = -1;
    timestamp.tm_year -= UT_EPOCH_YEAR;
    timestamp.tm_mon--;
    
    rc = em_get_id(state, name, &id);
    
    if(rc) {
      return UT_ERR_BAD_FILE;
    }
    
    state->levels[id].last_value = value;
    state->levels[id].last_update = mktime(&timestamp);
    
    /* set the last event time to now */
    state->levels[id].last_event = now;
    
  //loop until end of file
  } while(!feof(file));
	
  fclose(file);
  return UT_ERR_NONE;
} 

/* Saves a state to a file specified by path. Returns UT_ERR_NONE on
 * success or UT_ERR_BAD_PATH if the file cannot be written. */
int em_save(em_State *state, const char *path) {
  struct tm *time;
  FILE *file;
  int i;
	
  /* open the file */
  file = fopen(path, "w");

  /* check file is open */
  if(!file) {
    return UT_ERR_BAD_PATH;
  }

  /* write time and value of each level */
  for(i = 0; i < state->num_emotions; i++) {
  	
  	/* convert the timestamp into calander time */
  	time = gmtime(&state->levels[i].last_update);
  	
  	/* print the value and the date to the file */
    fprintf(file, "%10s %02d/%02d/%02d %02d:%02d:%02d  %lf\n", state->emotions[i].name, time->tm_mday, time->tm_mon + 1,
              time->tm_year + UT_EPOCH_YEAR, time->tm_hour, time->tm_min, time->tm_sec, state->levels[i].last_value);
  }
  
  /* close and return */
  fclose(file);
  return 0;
}

/* Returns the level of an emotion */
double em_get(em_State *state, int emotion) {
  double difference, num_decays, value;
  
  /* calculate time since last update */
  difference = difftime(time(NULL), state->levels[emotion].last_update);
  
  /* work out the number of decay times that have passed */
  num_decays = difference / (double) state->emotions[emotion].decay_time;
  
  /* caluclate the current value */
  value = state->levels[emotion].last_value  * pow(state->emotions[emotion].factor, num_decays);
  
  //dont let value go above the max
  if(value > state->emotions[emotion].max) {
    value = state->emotions[emotion].max;
  }
  
  return value;
}

/* Return the condition of an emotion */
em_condition em_get_condition(em_State *state, int emotion) {
int level;

level = em_get(state, emotion);

if(level > state->emotions[emotion].full) {
  return EM_COND_FULL;
}

else if(level < state->emotions[emotion].critical) {
  return EM_COND_CRITICAL;
}

else if(level < state->emotions[emotion].low) {
  return EM_COND_LOW;
}

else {
  return EM_COND_NORMAL;
}
}


/* Returns the average value of all the levels in a state */
double em_overall(em_State *state) {
  double total;
  int i;

  total = 0;
  	
  for(i = 0; i < state->num_emotions; i++) {
    total += em_get(state, i);
  }

  return total / (double) state->num_emotions;
}

/* Sets the level of an emotion to the value given. Returns UT_ERR_NONE on success
 * or UT_ERR_BAD_ARG if the value is not between zero and the maximum value  */
int em_set(em_State *state, int emotion, double value) {

  /* check bounds */
  if(value > state->emotions[emotion].max || value < 0) {
    return UT_ERR_BAD_ARG;
  }
	
  state->levels[emotion].last_value = value;
  state->levels[emotion].last_update = time(NULL);
  
  return UT_ERR_NONE;
}

/* Adds the value given to the level of an emotion but does not allow the
 * level to go above EM_MAX_LEVEL or below 0. */
int em_update(em_State *state, int emotion, double value) {
  double total;
	
  /* add the previous value to the update vale to get the total */
  total = value + em_get(state, emotion);
	
  /* if the total is above the maximum level then set the level to maximum */
  if (total > state->emotions[emotion].max) {
    em_set(state, emotion, state->emotions[emotion].max);
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

/* Checks for emotional events and stores the first one found in event.
 * Returns UT_ERR_NONE if an event was found or UT_ERR_EMPTY otherwise */
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
  	  	
  	    return UT_ERR_NONE;
  	  }
    } 
  }
  
  /* return no event */
  return UT_ERR_EMPTY;
}

/* Updates a state according to a reaction struct. */
int em_react(em_State *state, const em_Reaction *reaction) {
  
  /* set the emotion to the value if action is set */
  if(reaction->action == EM_ACTION_SET) {
  	return em_set(state, reaction->emotion, reaction->value);
  }
  
  /* update it if action is update */
  else if(reaction->action == EM_ACTION_UPDATE) {
    return em_update(state, reaction->emotion, reaction->value);
  }
  
  /* do nothing if action is none */
  else if(reaction->action == EM_ACTION_NONE) {
    return UT_ERR_NONE;
  }
  
  /* otherwise return an error */
  else {
    return UT_ERR_BAD_ACTION;
  }
}

/* Returns the number of an emotion given its name */
int em_get_id(em_State *state, const char *name, int *id) {
  int i;
  
  //loop through all emotions
  for(i = 0; i < state->num_emotions; i++) {
  	
  	//check if names match
  	if(strcmp(state->emotions[i].name, name) == 0){
 
  	  //if store number and return
  	  *id = i;
  	  return UT_ERR_NONE;
  	}
  }
  
  //if not found return bad arg
  return UT_ERR_BAD_ARG;
}

/* Returns a pointer to the name of an emotion given its number */
int em_get_name(em_State *state, int id, const char **name) {
  assert(id > 0);
  assert(id < state->num_emotions);
	
  //look name up in emotion array
  *name = state->emotions[id].name;
  return UT_ERR_NONE;
}

//store the list of emotions names of a state in an array
void em_get_names(em_State *state, const char **names) {
  int i;
	
  for(i = 0; i < state->num_emotions; i++) {
  	names[i] = state->emotions[i].name;
  }
}
