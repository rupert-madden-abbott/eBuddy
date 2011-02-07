/**
 * @file emotion.h
 * @author Rowan Saundry
 *
 * The emotion module is responsible for controlling the emotional state of the
 * eBuddy. An emotional state is created by calling em_init with an array of 
 * em_emotions. Once created, the emotion levels will begin to decay according
 * to the formula "value = initial_value * factor ^ time_period / decay_time".
 * The values will continue to decay even if the state is saved to a file.
 *
 * The file format the emotions are saved in contains one line for each file
 * and is in the format "name date time" where date is in the in the format
 * dd/mm/yy and time is in the format hh:mm:ss.
 */
 
#ifndef EM_H
#define EM_H

#include <time.h>
#include "utility.h"


/**
 * @define EM_NAME_LEN
 * 
 * Maximum length of an emotion name
 */
#define EM_NAME_LEN 12

/**
 * @define EM_LINE_ITEMS
 * 
 * The number of items on one line of the state file.
 */
#define EM_LINE_ITEMS 8

/**
 * @enum em_condition
 * 
 * The level of an emotion relative to it's full, low and critical values.
 *
 * Contains the following values:
 *
 * EM_COND_NORMAL       - The level is between low and full.
 * EM_COND_FULL         - The level is above full.
 * EM_COND_LOW          - The level is between critical and low.
 * EM_COND_CRITICAL     - The level is below critical.
 */
typedef enum em_condition {
  EM_COND_NORMAL = 0,
  EM_COND_FULL,
  EM_COND_LOW,
  EM_COND_CRITICAL
} em_condition;

/**
 * @enum em_action
 *
 * Represents an action that can be performed on an emotion level.
 *
 * EM_ACTION_NONE        - Does nothing.
 * EM_ACTION_SET         - Sets the emotion to the value (calls em_set).
 * EM_ACTION_UPDATE      - Updates the emotion  by the value (calls em_update).
 */
typedef enum em_action {
  EM_ACTION_NONE = 0,
  EM_ACTION_SET,
  EM_ACTION_UPDATE
} em_action;

/**
 * @struct em_emotion
 *
 * Represents a type of emotion and is used to initialise a state.
 * Contains the following fields:
 *
 * name        - The name of the emotion (string of any alphanum chars).
 * decay_time  - The time it takes for this emotion to decay in seconds.
 * factor      - The value the emotion will have after one deacy time.
 * event_time  - The amount of time in seconds between event notifications.
 * max         - The highest value the emotion can take.
 * full        - Above this value the emotion is full.
 * low         - Below this value the emotion is low.
 * critical    - Below this value the emotion is critical.
 */
typedef struct em_emotion {
  char name[EM_NAME_LEN];
  int decay_time;
  double factor;
  int event_time;
  double max;
  double full;
  double low;
  double critical;
} em_Emotion;

/**
 * @struct em_level
 *
 * Represents the current level of a single emotion and stores its
 * last event time.
 *
 * Contains the following fields:
 *
 * last_value      - The Value after the last update
 * last_update     - The time of the last update
 * last_event      - The time of the last event notification
 */
typedef struct em_level {
    double last_value;
    time_t last_update;
    time_t last_event;
} em_Level;

/**
 * @struct em_state
 *
 * Represents a set of emotions and their levels.
 *
 * Contains the following fields.
 *
 * emotions       - The array of emotions this state represents.
 * levels         - The levels of each emotion.
 * num_emotions   - The number of emotions.
 */
typedef struct em_state {
    const em_Emotion *emotions;
    em_Level *levels;
    int num_emotions;
} em_State;

/**
 * @struct em_event
 *
 * Represents an emotion event.
 *
 * Contains the following fields:
 *
 * type          - The type of the event
 * emotion       - The emotion the event applies to.
 */
typedef struct em_event {
  em_condition type;
  int emotion;
} em_Event;

/**
 * @struct em_reaction
 *
 * Represents an operation on a state.
 *
 * Contains the following fields:
 *
 * action             - The action to perform.
 * emotion            - The emotion to perform it on.
 * value              - The argument for the action.
 */
typedef struct em_reaction {
  em_action action;
  int emotion;
  double value;
} em_Reaction;


/**
 * Creates a new emotional state and returns a pointer to it. Values will be
 * initialised to their low value by default. Returns NULL if memory
 * allocation fails.
 *
 * @arg emotions The array of emotions the state will store
 * @arg num_emotions The size of the emotions array
 * @return A new emotion state pointer
 */
em_State *em_init(const em_Emotion *emotions, int num_emotions);

/**
 * Destroy an emotional state.
 *
 * @arg state The state to destroy
 */
void em_destroy(em_State *state);

/**
 * Resets a state to its default values. Each level will be set to low
 *
 * @arg state The state to reset.
 */
void em_reset(em_State *state);


/**
 * Loads a state from a file specified by path. The the number of emtions and
 * their names inside the file must match those in the array passed to init.
 * Returns UT_ERR_NONE on success, UT_ERR_BAD_PATH if the file cannot be read
 * or, UT_ERR_BAD_FILE if the file is not in the correct format.
 * 
 * @arg state The state to load
 * @arg path The path of the file to load from
 * @return ut_error_code
 */
ut_ErrorCode em_load(em_State *state, const char *path);

/**
 * Saves a state to a file specified by path. The file will be created if it
 * does not exist. Returns UT_ERR_NONE on success or UT_ERR_BAD_PATH if the
 * path cannot be written to.
 *
 * @arg state The state to save
 * @arg path The path of the file to save to
 * @return A ut_error_code
 */
ut_ErrorCode em_save(em_State *state, const char *path);

/**
 * Returns the current level of an emotion.
 *
 * @arg state The state containing the emotion
 * @arg emotion The number of the emotion to get
 * @return The level of the emotion
 */
double em_get(em_State *state, int emotion);

/**
 * Returns the current condition of an emotion.
 *
 * @arg state The state containing the emotion
 * @arg emotion The number of the emotion to check
 * @return The condition of the emotion
 */
em_condition em_get_condition(em_State *state, int emotion);

/**
 * Returns the average value of all the emotions in a state.
 *
 * @arg The state containing the emotions
 * @return The average emotion value
 */
double em_overall(em_State *state);

/**
 * Sets the level of an emotion to the value given. Returns UT_ERR_NONE on
 * success or UT_ERR_BAD_ARG if the value is not between zero and the
 * maximum value.
 *
 * @arg state The state containing the emotion
 * @arg emotion The number of the emotion to set
 * @arg value The value to set the emotion to
 * @return A ut_error_code
 */
ut_ErrorCode em_set(em_State *state, int emotion, double value);

/**
 * Adds the value given to the level of an emotion but does not allow the
 * level to go above EM_MAX_LEVEL or below 0. Values can be negative.
 *
 * @arg emotion The number of the emotion to set
 * @arg value The value to add to the emotion
 */
ut_ErrorCode em_update(em_State *state, int emotion, double value);

/**
 * Checks for emotional events and stores the first one found in event.
 * Returns UT_ERR_NONE if an event was found or UT_ERR_EMPTY otherwise
 *
 * @arg state The state to check for events
 * @arg event A pointer to the struct to store the event in
 */
ut_ErrorCode em_check(em_State *state, em_Event *event);

/**
 * Modifies a state according to a reaction struct. Returns UT_ERR_NONE on
 * success or UT_ERR_BAD if action is set and the value is out of bounds. 
 *
 * @arg state The state to modify
 * @arg reaction The action to perform
 * @return A ut_error_code
 */
ut_ErrorCode em_react(em_State *state, const em_Reaction *reaction);

/**
 * Gets the number of an emotion given its name. Returns UT_ERR_NONE on
 * success or UT_BAD_ARG if an invalid name is given.
 *
 * @arg state The state containing the emotion.
 * @arg name A string containing the name of the emotion
 * @arg id A pointer to the integer that will hold the emotion number
 * @return A ut_error_code
 */
ut_ErrorCode em_get_id(em_State *state, const char *name, int *id);

/*
 * Gets the name of an emotion given its number. Does not check if the
 * emotion number is out of bounds. It is the responsibility of the calling
 * function to ensure that the length of the name is smaller than the
 * buffer size.
 *
 * @arg state The state containing the emotion.
 * @arg id The emotion number
 * @arg name The buffer that will hold the name of the emotion
 * @return A ut_error_code
 */
ut_ErrorCode em_get_name(em_State *state, int id, const char **name);

/*
 * Gets the names of all the emotions in the state. The names are not copied,
 * a pointer is returned to the name field in the array the state was
 * initialised with.
 *
 * @arg state The state containing the names
 * @arg names The array of character pointers that will store the names
 */
void em_get_names(em_State *state, const char **names);

#endif
