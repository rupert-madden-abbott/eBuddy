/* the react loop is the eBuddys main loop. it continually reads
 * and responds to input and emotion events by interpreting a
 * set of reaction tables stored below */

#include "react.h"


//list of alerts for each emotion
//each reaction is a gesture function and a text string for the lcd
const EmotionAction rct_em_action[NUM_EMOTIONS] = {
	
//full alert	 					low	alert					critical alert					critical mode
  {{gsi_eyeflash,	"*burp*"},		{gsi_eyeflash,	"hungry"},	{gsi_raise_arms,	"feed me"},		MODE_NONE},	//hunger
  {{gsi_eyeflash,	"aaauuuoowhh"},	{gsi_raise_arms,"*yaawn*"},	{gsi_raise_arms,	"*yaaaaaawn*"},	MODE_SLEEP},//energy
  {{NULL,			""},			{gsi_eyeflash, 	"euhg"},	{gsi_shake_head,	"eeeewwwwww"},	MODE_NONE},	//cleanliness
  {{NULL,			":)"},			{gsi_raise_arms, "hey"},	{gsi_fun_level1,	"helloooo"}, 	MODE_NONE},	//social
  {{gsi_fun_level1, "wooooo"},		{gsi_eyeflash,	":("},		{gsi_shake_head,	"booooring"},	MODE_NONE}	//fun
};

//list of alerts and updates for each input
const InputAction rct_in_action[IN_NUM_INPUTS] = {

  {{EM_ACTION_UPDATE,	EMO_HUNGER,	50},	{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_shake_head,	"*burp*"},				{gsi_eyeflash,	"munch munch"},		{gsi_shake_head,	"yummm!!!"},	MODE_NONE}, //nuts and bolts
  {{EM_ACTION_UPDATE,	EMO_ENERGY,	-25},	{EM_ACTION_UPDATE,	EMO_FUN,	15},
  {gsi_fun_level1,	"*glug* *glug*"}, 		{gsi_eyeflash,	"glug"}, 			{gsi_shake_head,	"yuck"},		MODE_NONE}, //oil can
  {{EM_ACTION_UPDATE,	EMO_FUN,	-25},	{EM_ACTION_UPDATE,	EMO_ENERGY,	15},
  {gsi_raise_arms,	"buzz buzz buzz"},		{gsi_eyeflash,	"buzz..."},			{gsi_shake_head, "..."},			MODE_NONE},	//battery
  
  
  {{EM_ACTION_UPDATE,	EMO_SOCIAL,	20},	{EM_ACTION_UPDATE,	EMO_FUN,	5},
  {gsi_eyeflash,	"meh"},					{gsi_move_arms,	"tee he he"},		{gsi_happy_level1,	"yeaaaa"},			MODE_NONE},	//force sensor
  {{EM_ACTION_NONE,		EMO_ENERGY,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_shake_head,	":("},					{gsi_move_arms,		"yawn"},		{gsi_raise_arms,	"yaaawn"},			MODE_SLEEP},//sleep sensor
  
  
  {{EM_ACTION_NONE,		EMO_FUN,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_shake_head,	""},					{gsi_raise_left,		":D"},				{gsi_raise_left,	":)"},			MODE_GUESS},	//left hand
  {{EM_ACTION_NONE,		EMO_FUN,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_shake_head,	""},					{gsi_raise_right,		":D"},				{gsi_raise_right,	":)"},			MODE_NONE},	//right hand 
  
  
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},  
  {NULL,			""},					{NULL,			""},				{gsi_eyeflash,		"i am a robot"},	MODE_NONE},	//power on
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {NULL,			""},					{NULL,			""},				{gsi_wave_right,	"bye bye"},			MODE_END},	//power off
  
  
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {NULL,			""},					{NULL,			""},				{gsi_eyeflash,		"demo"},			MODE_DEMO},	//demo key
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {NULL,			""},					{NULL,			""},				{gsi_eyeflash,		"debug"},			MODE_DEBUG}	//debug key
};

//Reaction to notifications
const gsi_Reaction rct_msg_action = {gsi_shake_head, "beep beep"};

//Sleeping loop actions
const InputAction rct_sleep_action = {{EM_ACTION_UPDATE,	EMO_ENERGY,	1},	{EM_ACTION_NONE,	EMO_NONE,	20},
  {gsi_shake_head,	"..."},		{gsi_eyeflash,	"zzzzz"},		{gsi_eyeflash, "zzZZZzZz"},		MODE_END};



//Main loop
int rct_main(em_State *emotions, qu_queue *notifications, ph_handle *phhandle) {
  const EmotionAction *em_action;
  const InputAction *in_action;
  nt_message *message;
  em_Event emotion_event; 
  em_condition condition;
  in_input_type input_event;
  int running, rc;

  //keep looping until shutdown signal
  running = 1;
  
  while(running) {

    //look for input events
    input_event = in_get_input();
  
    //react to events
    if(input_event) {
  	
      //get action from table
      in_action = &rct_in_action[input_event - 1];
  	
      //get condition of primary emotion before update
      if(in_action->primary_emotion.emotion != EMO_NONE) {
        condition = em_get_condition(emotions, in_action->primary_emotion.emotion);
      }
  	
      //if emotion is none use critical gesture
      else {
        condition = EM_COND_CRITICAL;
      }
  	
      //update primary emotion
  	  rc = em_react(emotions, &in_action->primary_emotion);
  	
  	  //check for errors in reaction table
      assert(rc ==0);
  	
  	  //update secondary emotion
      rc = em_react(emotions, &in_action->secondary_emotion);
    
      //check for errors in reaction table
      assert(rc ==0);
    
      //if emotion was full do full gesture
      if(condition == EM_COND_FULL) {
        gsi_react(&in_action->full_gesture, phhandle);
      }
    
      //if it was normal do normal gesture
      else if(condition == EM_COND_NORMAL) {
        gsi_react(&in_action->normal_gesture, phhandle);
      }
    
      //otherwise do low gesture
      else {
        gsi_react(&in_action->low_gesture, phhandle);
      }
      
      //check for shutdown signal
      if(in_action->mode == MODE_END) {
        running = 0;
      }
    
      //if the input triggers a mode change and emotion level isnt full switch mode
      else if(in_action->mode && condition != EM_COND_FULL) {
        rc = mode_run(in_action->mode, emotions, notifications);
      
        //return errors to parent mode
        if(rc) {
          return rc;
        }
      }
    }
 
    //look for emotion events
    rc = em_check(emotions, &emotion_event);
  
    //react to events if there was one
    if(!rc) {
  	
  	  //get the correct set of reations
  	  em_action = &rct_em_action[emotion_event.emotion];
  	
  	  //do full gesture if condition is full
      if(emotion_event.type == EM_COND_FULL) {
        gsi_react(&em_action->full_gesture);
      }
    
    //do low gesture if condition is low
      else if(emotion_event.type == EM_COND_LOW) {
        gsi_react(&em_action->low_gesture, phhandle);
      }
    
      //do critical gesture if condition is critical
      else {
        gsi_react(&em_action->critical_gesture, phhandle);
        
        //if critical mode is kill end main loop
        if(em_action->critical_mode == MODE_END) {
          running = 0;
        }
        
        //check for other mode changes
        else if(em_action->critical_mode) {
          mode_run(em_action->critical_mode, emotions, notifications);
        }
      }
    }
  
    //get notification events
    message = qu_pop(notifications);
  
    //do a gesture and print the text on the screen
    if(message) {
      gsi_react(&rct_msg_action, phhandle);
      gsi_printLCD(message->text, phhandle);
    }
  
    sleep(1);
  }
  
  return ERR_CLOSE;
}


//Sleeping loop
int rct_sleep(em_State *emotions, qu_queue *notifications) {
  nt_message *message;
  em_Event emotion_event; 
  em_condition condition;
  in_input_type input_event;
  int asleep, rc;

  //keep looping until woken up
  asleep = 1;
  
  while(asleep) {

    //look for input events
    input_event = in_get_input();
  
    //wake up on button press or force sensor
    if(input_event == INPT_LEFT_HAND || input_event == INPT_RIGHT_HAND || input_event == INPT_FORCE) {
  	  asleep = 0;
    }
    
    //shutdown on power button
    if(input_event == INPT_POWER_OFF) {
  	  return ERR_CLOSE;
    }
 
    //look for emotion events but don't respond to them
    rc = em_check(emotions, &emotion_event);
  
    //get notification events
    message = qu_pop(notifications);
  
    //print the text on the screen
    if(message) {
      gsi_printLCD(message->text);
    }
  	
    //get condition of primary sleep emotion
    if(rct_sleep_action.primary_emotion.emotion != EMO_NONE) {
      condition = em_get_condition(emotions, rct_sleep_action.primary_emotion.emotion);
    }
  	
    //if emotion is none use full gesture
    else {
      condition = EM_COND_FULL;
    }
  	
    //update primary emotion
  	rc = em_react(emotions, &rct_sleep_action.primary_emotion);
  	
  	//check for errors in reaction table
    assert(rc ==0);
  	
  	//update secondary emotion
    rc = em_react(emotions, &rct_sleep_action.secondary_emotion);
    
    //check for errors in reaction table
    assert(rc ==0);
    
    //if emotion was full do full gesture
    if(condition == EM_COND_FULL) {
      gsi_react(&rct_sleep_action.full_gesture);
      
      //check for wake up signal
      if(rct_sleep_action.mode == MODE_END) {
        asleep = 0;
      }
    
      //if the input triggers a mode change switch to that mode
      else if(rct_sleep_action.mode) {
        rc = mode_run(rct_sleep_action.mode, emotions, notifications);
      
        //return errors to parent mode
        if(rc) {
          return rc;
        }
      }
    }
    
    //if it was normal do normal gesture
    else if(condition == EM_COND_NORMAL) {
      gsi_react(&rct_sleep_action.normal_gesture);
    }
    
    //otherwise do low gesture
    else {
      gsi_react(&rct_sleep_action.low_gesture);
    }
  
    sleep(5);
  }
  
  //wake up animation
  gsi_raise_arms();
  
  return ERR_NONE;
}
