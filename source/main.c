#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"
#include "phidget.h"
#include "input.h"
#include "emotion.h"
#include "queue.h"
#include "notify.h"
#include "mode.h"
#include "main.h"

//list of emotions and their decay times etc
//each emotion will decay to 1*factor of its original value
//after one life. when the level is above full or below low
//the user will be alerted once every alert time. all times
//are in seconds.
const em_Emotion main_emotions[] = {
	
//life				factor		alert			max		full	low		critical
  {UT_HOUR * 24,	0.2,		UT_MIN * 1,		100,	90,		30,		10},	//hunger
  {UT_HOUR * 18,	0.5,		UT_MIN * 1,		100,	90,		20,		10},	//energy
  {UT_HOUR * 24,	0.3,		UT_MIN * 1,		150,	100,	50,		10},	//cleanliness
  {UT_HOUR * 2,		0.9,		UT_MIN * 1,		100,	98,		40,		20},	//social
  {UT_HOUR * 2,		0.7,		UT_MIN * 2,		100,	98,		20,		5}		//fun
};


int main(void) {
  em_State *emotions;
  qu_queue *notifications;
  error_code rc;

  //create a new emotion state using the emotion table
  emotions = em_create(main_emotions, NUM_EMOTIONS);

  if(!emotions) {
    printf("Error initialising emotions\n");
    printf("Check enough memory is available and try again\n");
  	exit(1);
  }
  
  //load the emotion values from the last session
  rc = em_load(emotions, EM_STATE_PATH);
  
  //if file is corrupt keep running using defaults
  if(rc == ERR_BAD_FILE) {
    printf("Error: state file is corrupt\n");
    printf("Reseting to defaults\n");
  }
  
  //if file doesn't exist but path is valid keep running
  else if(rc == ERR_BAD_PATH && test_path(EM_STATE_PATH)) {
    printf("Could not find state file\n");
    printf("Reseting to defaults\n");
  }
  
  //all other errors are fatal
  else if(rc) {
    printf("Error reading state file\n");
    exit(1);
  }
  
  //initialise the phidgets
  rc = ph_init(CONFIG_PATH);
  if(rc) {
  	printf("Error initialising phidgits\n");
  	exit(1);
  }
  
  //create the notification queue
  notifications = qu_init();
  
  if(!notifications) {
    printf("Could not initialise notification queue\n");
    printf("Check enough memory is available and try again\n");
    exit(1);
  }
  
  //initialise notification system
  //rc = nt_init(notifications, NT_CONFIG);
  
  if(rc) {
  	printf("Error initialising notification system\n");
  	exit(1);
  }
  
  //enter main interactive mode
  rc = mode_run(STARTUP_MODE, emotions, notifications);
  
  //finalise and unload all modules
  printf("Shutting down\n");
  em_save(emotions, EM_STATE_PATH);
  em_destroy(emotions);
  //nt_destroy();
  ph_destruct();
  return 0;
}