#ifndef MAIN_H
#define MAIN_H

#define CONFIG_PATH "conf/"
#define NT_CONFIG CONFIG_PATH "notify.json"
#define EM_STATE_PATH CONFIG_PATH "em_state"

#define STARTUP_MODE MODE_MAIN

//number of emotions in the emotion table
#define NUM_EMOTIONS 5

typedef enum run_mode {
  MODE_NONE = 0,
  MODE_KILL = -1,
  MODE_MAIN,
  MODE_DEMO,
  MODE_GUESS,
} run_mode;

//emotion names and table row numbers
typedef enum emotion_type {
  EMO_NONE = -1,
  EMO_HUNGER = 0,
  EMO_ENERGY = 1,
  EMO_CLEAN = 2,
  EMO_SOCIAL = 3,
  EMO_FUN = 4
} emotion_type;

//represents the action to take when an input is recieved
typedef struct input_action {
  const em_Reaction primary_emotion;
  const em_Reaction secondary_emotion;
  const gsi_Reaction full_gesture;
  const gsi_Reaction normal_gesture;
  const gsi_Reaction low_gesture;
  const run_mode mode;
} InputAction;

//represents the gesture to perform when an emotion reaches a certain level
typedef struct emotion_action {
  const gsi_Reaction full_gesture;
  const gsi_Reaction low_gesture;
  const gsi_Reaction critical_gesture;
  const run_mode critical_mode;
} EmotionAction;

//not currently used
typedef struct PhidgetHandle {
  void *example;
} PhidgetHandle;

//list of emotions and their decay times etc
//each emotion will decay to 1*factor of its original value
//after one life (in hours). when the level is above full or
//below once low every alert time (in minutes)
const em_Emotion EMOTIONS[] = {
	
//life		factor	alert	max		full	low		critical
  {24,		0.5,	10,		100,	90,		30,		10},	//hunger
  {18,		0.5,	10,		100,	90,		20,		10},	//energy
  {6,		0.5,	5,		150,	100,	50,		10},	//cleanliness
  {6,		0.5,	10,		100,	98,		40,		20},	//social
  {24,		0.5,	20,		100,	98,		20,		5}		//fun
};

//list of alerts for each emotion
//each reaction is a gesture function and a text string for the lcd
const EmotionAction emotion_actions[] = {
	
//full alert	 					low	alert					critical alert					critical mode
  {{gsi_eyeflash,	"*gurgle*"},	{NULL,			"hungry"},	{NULL,	"feed me"},					MODE_NONE},	//hunger
  {{gsi_eyeflash,	"*yawn*"},		{gsi_raise_arms,"*yaawn*"},	{gsi_raise_arms,	"aaauuuoowhh"},	MODE_NONE},	//energy
  {{NULL,			""},			{gsi_eyeflash, 	"euhg"},	{gsi_shake_head,	"eeeewwwwww"},	MODE_NONE},	//cleanliness
  {{NULL,			"lurrrrve"},	{gsi_raise_arms, "hey"},	{gsi_fun_level1,	"helloooo"}, 	MODE_NONE},	//social
  {{gsi_fun_level1, "wooooo"},		{gsi_eyeflash,	"meh"},		{gsi_shake_head,	"boring"},		MODE_NONE}	//fun
};

//list of alerts and updates for each input
const InputAction input_actions[] = {

  {{EM_ACTION_UPDATE,	EMO_HUNGER,	50},	{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_shake_head,	"no thanks"},	{gsi_eyeflash,	"mmmm"},		{gsi_shake_head,	"yummm!!!"},	MODE_NONE}, //nuts and bolts
  {{EM_ACTION_UPDATE,	EMO_ENERGY,	-15},	{EM_ACTION_UPDATE,	EMO_FUN,	20},
  {gsi_shake_head,	"i can't"}, 	{gsi_eyeflash,	"glug"}, 		{gsi_fun_level1,	"yay"},			MODE_NONE}, //oil can
  {{EM_ACTION_UPDATE,	EMO_HUNGER,	20},	{EM_ACTION_UPDATE,	EMO_ENERGY,	20},
  {gsi_shake_head,	"no thanks"},		{gsi_eyeflash,	"buzzz"},		{gsi_fun_level1, "buzzzzzz"},	MODE_NONE},	//battery
  {{EM_ACTION_UPDATE,	EMO_SOCIAL,	15},	{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_eyeflash,	"meh"},			{gsi_raise_arms,	"gurrrr"},	{gsi_happy_level1,	"yeaaaa"},		MODE_NONE},	//touch sensor
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {NULL,			""},			{NULL,			""},			{NULL,				""},			MODE_KILL}	//power button
};

const gsi_Reaction message_action = {gsi_shake_head, "beep beep"};

int enter_mode(run_mode mode, em_State *emotions, qu_queue *notifications);
int run_demo(em_State *emotions, qu_queue *notifications);
int run_main(em_State *emotions, qu_queue *notifications);

#endif
