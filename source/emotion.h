#ifndef EM_H
#define EM_H

#include <time.h>

//number of items on one line of the state file
#define EM_LINE_ITEMS 8

//maximum name length of an emotion
#define EM_NAME_LEN 12

/* the level of an emotion relative to it's event values */
typedef enum em_condition {
  EM_COND_NORMAL = 0,		/* the level is between low and full */
  EM_COND_FULL,				/* the level is above full */
  EM_COND_LOW,				/* the level is between critical and low */
  EM_COND_CRITICAL			/* the level is below critical */
} em_condition;

/* represents a type of reaction */
typedef enum em_action {
  EM_ACTION_NONE = 0,		/* do nothing */
  EM_ACTION_SET,			/* set the emotion to the value */
  EM_ACTION_UPDATE			/* update the emotion  by the value */
} em_action;

/* represents a type of emotion and gives decay times etc. */
typedef struct em_emotion {
  char name[EM_NAME_LEN];	//the id of this emotion (string of any alphanum chars)
  int decay_time;			//the time it takes for this emotion to decay in seconds
  double factor;			//the value the emotion will have after one deacy time (eg 1/2, 0)	
  int event_time;			//the amount of time in seconds between event notifications
  double max;				//the highest value an emotion can take
  double full;				//above this value the emotion is full
  double low;				//below this value the emotion is low
  double critical;			//below this value the emotion is critical
} em_Emotion;

/* represents the current level of an emotion */
typedef struct em_level {
	double last_value;		/* value after last update */
	time_t last_update;		/* time of last update */
	time_t last_event;		/* time of last event notification */
} em_Level;

/* represents a set of emotions and their levels */
typedef struct em_state {
	const em_Emotion *emotions;	/* the array of emotions this state represents */
	em_Level *levels;			/* the levels of the emotions */
	int num_emotions;			/* the number of emotions */
} em_State;

/* represents an emotion event */
typedef struct em_event {
  em_condition type;		/* the condition the emotion is in */
  int emotion;				/* the emotion the condition applies to */
} em_Event;

/* represents a change to a state */ 
typedef struct em_reaction {
  em_action action;			/* the action to perform */
  int emotion;				/* the emotion to perform it on */
  double value;				/* the argument for the action */
} em_Reaction;


/* allocate and return a pointer to a new emotional state */
em_State *em_create(const em_Emotion *values, int num_values);

/* free up a state */
void em_destroy(em_State *state);

/* load an emotional state from a file */
int em_load(em_State *state, const char *path);

/* save state to a file */
int em_save(em_State *state, const char *path);

/* returns the level of an emotion */
double em_get(em_State *state, int emotion);

/* return the condition of an emotion */
em_condition em_get_condition(em_State *state, int emotion);

/* returns the average value of all the levels in a state */
double em_overall(em_State *state);

/* sets the level of an emotion to the value given. will not allow the
 * level to go above EM_MAX_LEVEL or below 0. returns the condition of
 * the emotion after the update */
int em_set(em_State *state, int emotion, double value);

/* adds the value given to the level of an emotion. will not allow the
 * level to go above EM_MAX_LEVEL or below 0. returns the condition of
 * the emotion after the update */
int em_update(em_State *state, int emotion, double value);

/* checks for emotional events and stores the first one found in event.
 * returns 0 if an event was found or 1 otherwise */
int em_check(em_State *state, em_Event *event);

/* updates a state according to the a reaction struct. */
int em_react(em_State *state, const em_Reaction *reaction);

//reset a state back to its defaults
void em_reset(em_State *state);

//return the number of an emotion given its name
int em_get_id(em_State *state, const char *name, int *id);

//return a pointer to the name of an emotion given its number
int em_get_name(em_State *state, int id, const char **name);

//store the list of emotions names of a state in an array
void em_get_names(em_State *state, const char **names);

#endif
