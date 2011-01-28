/* the react loop is the eBuddys main loop. it continually reads
 * and responds to input and emotion events by interpreting a
 * set of reaction tables stored below */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <phidget21.h>
#include <jansson.h>

#include "utility.h"
#include "phidget.h"
#include "input.h"
#include "emotion.h"
#include "gesture.h"
#include "gesture_interface.h"
#include "config.h"
#include "queue.h"
#include "notify.h"
#include "mode.h"
#include "main.h"
#include "react.h"


//list of alerts for each emotion
//each reaction is a gesture function and a text string for the lcd
const EmotionAction rct_em_action[NUM_EMOTIONS] = {
	
//full alert	 					low	alert					critical alert					critical mode
  {{gsi_eyeflash,	"*gurgle*"},	{NULL,			"hungry"},	{NULL,	"feed me"},					MODE_NONE},	//hunger
  {{gsi_eyeflash,	"*yawn*"},		{gsi_raise_arms,"*yaawn*"},	{gsi_raise_arms,	"aaauuuoowhh"},	MODE_NONE},	//energy
  {{NULL,			""},			{gsi_eyeflash, 	"euhg"},	{gsi_shake_head,	"eeeewwwwww"},	MODE_NONE},	//cleanliness
  {{NULL,			"lurrrrve"},	{gsi_raise_arms, "hey"},	{gsi_fun_level1,	"helloooo"}, 	MODE_NONE},	//social
  {{gsi_fun_level1, "wooooo"},		{gsi_eyeflash,	"meh"},		{gsi_shake_head,	"boring"},		MODE_NONE}	//fun
};

//list of alerts and updates for each input
const InputAction rct_in_action[IN_NUM_INPUTS] = {

  {{EM_ACTION_UPDATE,	EMO_HUNGER,	50},	{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_shake_head,	"no thanks"},		{gsi_eyeflash,	"mmmm"},		{gsi_shake_head,	"yummm!!!"},	MODE_NONE}, //nuts and bolts
  {{EM_ACTION_UPDATE,	EMO_ENERGY,	-15},	{EM_ACTION_UPDATE,	EMO_FUN,	20},
  {gsi_shake_head,	"i can't"}, 		{gsi_eyeflash,	"glug"}, 		{gsi_fun_level1,	"yay"},			MODE_NONE}, //oil can
  {{EM_ACTION_UPDATE,	EMO_HUNGER,	20},	{EM_ACTION_UPDATE,	EMO_ENERGY,	20},
  {gsi_shake_head,	"no thanks"},		{gsi_eyeflash,	"buzzz"},		{gsi_fun_level1, "buzzzzzz"},		MODE_NONE},	//battery
  
  
  {{EM_ACTION_UPDATE,	EMO_SOCIAL,	15},	{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_eyeflash,	"meh"},				{gsi_raise_arms,	"gurrrr"},	{gsi_happy_level1,	"yeaaaa"},		MODE_NONE},	//force sensor
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_eyeflash,	"meh"},				{gsi_raise_arms,	"gurrrr"},	{gsi_happy_level1,	"yeaaaa"},		MODE_NONE},	//touch sensor
  
  
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_eyeflash,		"game"},		{NULL,			""},			{NULL,				""},			MODE_NONE},	//left hand
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_eyeflash,		"game"},		{NULL,			""},			{NULL,				""},			MODE_NONE},	//right hand 
  
  
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},  
  {gsi_eyeflash,		"i am a robot"},{NULL,			""},			{NULL,				""},			MODE_NONE},	//power on
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_eyeflash,		"bye bye"},		{NULL,			""},			{NULL,				""},			MODE_KILL},	//power off
  
  
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_eyeflash,		"demo"},		{NULL,			""},			{NULL,				""},			MODE_DEMO},	//demo key
  {{EM_ACTION_NONE,		EMO_NONE,	0},		{EM_ACTION_NONE,	EMO_NONE,	0},
  {gsi_eyeflash,		"debug"},		{NULL,			""},			{NULL,				""},			MODE_DEBUG}	//debug key
};

//reaction to notifications
const gsi_Reaction rct_msg_action = {gsi_shake_head, "beep beep"};



//Main loop
int rct_main(em_State *emotions, qu_queue *notifications) {
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
  	
      //if emotion is none use full gesture
      else {
        condition = EM_COND_FULL;
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
      
      //check for shutdown signal
      if(in_action->mode == MODE_KILL) {
        running = 0;
      }
    
      //if the input triggers a mode change switch to that mode
      else if(in_action->mode) {
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
        gsi_react(&em_action->low_gesture);
      }
    
      //do critical gesture if condition is critical
      else {
        gsi_react(&em_action->critical_gesture);
        
        //if critical mode is kill end main loop
        if(em_action->critical_mode == MODE_KILL) {
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
      gsi_react(&rct_msg_action);
      gsi_printLCD(message->text);
    }
  
    sleep(1);
  }
  
  return ERR_NONE;
}


//sleeping loop
int rct_sleep(em_State *emotions, qu_queue *notifications) {
  //nt_message *message;
  //em_Event emotion_event; 
  //em_condition condition;
  //int input_event, rc;
  
  //print zzz
  
  //wait a few minutes
  //do a sellping animation
  return ERR_NONE;
}
