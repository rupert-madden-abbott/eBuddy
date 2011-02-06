/* Impliments functions from phidgit.h, input.h and gesture.h
 * using the simulation module instead of the phidgits library.
 * Does not have a header file as it uses those from the modules
 * it simulates. */

#include "sim_wrapper.h"

//virtual input reader
sim_Reader sim_input_reader;

//starts a simulation input reader using stin
int ph_init(const char *config_path, ph_handle *phhandle) {
  int rc;

  //initialise input reader
  rc = sim_in_init(&sim_input_reader);

  //check for errors
  if(rc) {
    return ERR_UNKNOWN;
  }
  
  return 0;
}

//these funtions have just been added to make the simulation compile
//something needs to be doen with them
int gsi_gesture_init(ph_handle *handle) {
  return 0;
}
int gsi_gesture_close(ph_handle *handle) {
  return 0;
}

int in_input_init(ph_handle *handle) {
  return 0;
}

//destroys the simulation input reader
int ph_destruct(ph_handle *phhandle) {
  sim_in_destroy(&sim_input_reader);
  return 0;
}

//read input from the input reader
int in_get_input(void) {
  int last_input;
  
  //get input from the reader
  sim_in_read(&sim_input_reader, &last_input);
  
  //return the input
  return last_input;
}

//output the given reaction to the user in text form
int gsi_react(const gsi_Reaction *resp, ph_handle *phhandle) {
	
  //check message is not null or null string
  if(resp->message && *(resp->message)) {
  	
  	//print message to the lcd screen
    gsi_printLCD(resp->message, phhandle);
  }
  
  //if a gesture is given run it
  if(resp->gesture) {
    resp->gesture(phhandle);
  }
  
  return 0;
}

int gsi_printLCD(const char* str, ph_handle *phhandle) {
  printf("-- %s --\n", str);
  
  return 0;
}

int gsi_happy_level1(ph_handle *phhandle) {
  printf("*smiles*\n");
  return 0;
}

int gsi_fun_level1(ph_handle *phhandle) {
  printf("*fun*\n");
  return 0;
}

int gsi_sound(const char *sound, int repeat) {
  printf("*sound*\n");
  return 0;
}

int gsi_test(ph_handle *phhandle) {
  printf("*test*\n");
  return 0;
}

int gsi_shake_head(ph_handle *phhandle) {
  printf("*shakes head*\n");
  return 0;
}

int gsi_move_arms(ph_handle *phhandle) {
  printf("*moves arms*\n");
  return 0;
}

int gsi_raise_arms(ph_handle *phhandle)  {
  printf("*raises arms*\n");
  return 0;
}

int gsi_eyeflash(ph_handle *phhandle) {
  printf("*flashes eyes*\n");
  return 0;
}

int gsi_wave_right(ph_handle *phhandle) {
  printf("*waves with right hand*\n");
  return 0;
}

int gsi_wave_left(ph_handle *phhandle) {
  printf("*waves with left hand*\n");
  return 0;
}

int gsi_raise_right(ph_handle *phhandle) {
  printf("*raises right hand*\n");
  return 0;
}
int gsi_raise_left(ph_handle *phhandle) {
  printf("*raises left hand*\n");
  return 0;
}
