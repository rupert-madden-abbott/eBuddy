#ifndef MAIN_H
#define MAIN_H

#define CONFIG_PATH "conf/"
#define EM_STATE_PATH CONFIG_PATH "em_state"

/* number of emotions in the emotion table */
#define NUM_EMOTIONS 2

typedef enum run_mode {
  MODE_NONE = 0,
  MODE_KILL = -1,
  MODE_INT,
  MODE_GUESS,
} run_mode;

//TODO add all emotion names

/* emotion names */
typedef enum emotion_type {
  EMO_HUNGER,
  EMO_FUN
} emotion_type;

/* represents the action to take when an input is recieved */
typedef struct input_action {
  em_Reaction primary_emotion;
  em_Reaction secondary_emotion;
  gsi_Reaction full_gesture;
  gsi_Reaction normal_gesture;
  gsi_Reaction low_gesture;
  run_mode mode;
} InputAction;

/* represents the gesture to perform when an emotion reaches a certain level */
typedef struct emotion_action {
  gsi_Reaction full_gesture;
  gsi_Reaction low_gesture;
  gsi_Reaction critical_gesture;
  run_mode critical_mode;
} EmotionAction;

/* not currently used */
typedef struct PhidgetHandle {
  void *example;
} PhidgetHandle;

//TODO add emotion values to table

/* list of emotions and their decay times etc */
const em_Emotion EMOTIONS[] = {
  {24,		0.5,	10,		800,	200,	100},	/* hungar */
  {0,		0,		0,		0,		0,		0}
};


//TODO add reactions for inputs and emotions

/* list of reactions for each emoton */
const EmotionAction emotion_actions[] = {
  {{NULL,	""}, {NULL,	""}, {NULL,	""},	MODE_NONE}		/* hunger */
};

const InputAction input_actions[] = {
  {{0,	0,	0},	{0,	0,	0}, {NULL,	""}, {NULL,	""}, {NULL,	""},	MODE_NONE}	/* somthing happens */

};


void end(void);
int main_loop(void);
int enter_mode(const char *config, int *input_buffer, em_State *emotions, qu_queue *message, int mode);

#endif
