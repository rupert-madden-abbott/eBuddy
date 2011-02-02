/* Impliments functions from phidgit.h, input.h and gesture.h
 * using the simulation module instead of the phidgits library.
 * Does not have a header file as it uses those from the modules
 * it simulates. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "utility.h"
#include "phidget.h"
#include "input.h"
#include "gesture.h"
#include "gesture_interface.h"
#include "simulation.h"

//virtual input reader
sim_Reader sim_input_reader;

//starts a simulation input reader using stin
int ph_init(const char *config_path) {
  int rc;

  //initialise input reader
  rc = sim_in_init(&sim_input_reader);

  //check for errors
  if(rc) {
    return ERR_UNKNOWN;
  }
  
  return 0;
}

//destroys the simulation input reader
int ph_destruct(void) {
  sim_in_destroy(&sim_input_reader);
  return 0;
}

//read input from the input reader
int in_get_input() {
  int last_input;
  
  //get input from the reader
  sim_in_read(&sim_input_reader, &last_input);
  
  //return the input
  return last_input;
}

//output the given reaction to the user in text form
int gsi_react(const gsi_Reaction *resp) {
	
  //check message is not null or null string
  if(resp->message && *(resp->message)) {
  	
  	//print message to the lcd screen
    gsi_printLCD(resp->message);
  }
  
  //if a gesture is given run it
  if(resp->gesture) {
    resp->gesture();
  }
  
  return 0;
}

int gsi_printLCD(const char* str) {
  printf("-- %s --\n", str);
  
  return 0;
}

int gsi_happy_level1(void) {
  printf("*smiles*\n");
  return 0;
}

int gsi_fun_level1(void) {
  printf("*fun*\n");
  return 0;
}

int gsi_notification(void) {
  printf("*notifies*\n");
  return 0;
}

int gsi_test(void) {
  printf("*test*\n");
  return 0;
}

int gsi_shake_head(void) {
  printf("*shakes head*\n");
  return 0;
}

int gsi_move_arms(void) {
  printf("*moves arms*\n");
  return 0;
}

int gsi_raise_arms(void)  {
  printf("*raises arms*\n");
  return 0;
}

int gsi_eyeflash(void) {
  printf("*flashes eyes*\n");
  return 0;
}

int gsi_wave_right(void) {
  printf("*waves with right hand*\n");
  return 0;
}

int gsi_wave_left(void) {
  printf("*waves with left hand*\n");
  return 0;
}

int gsi_raise_right(void) {
  printf("*raises right hand*\n");
  return 0;
}
int gsi_raise_left(void) {
  printf("*raises left hand*\n");
  return 0;
}