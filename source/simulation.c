/* the simulation module is used by the simulation
 * wrapper to allow it to emulate the phidgits. although
 * it does not depend on the phidgit library, it must be included
 * as needs access to the input codes defined in input.h */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <phidget21.h>

#include "utility.h"
#include "input.h"
#include "simulation.h"


//create and start a new input reader
int sim_in_init(sim_Reader *reader) {
  int rc;
	
  //clear input buffer
  reader->buffer = 0;
  
  //set mode to running
  reader->mode = SIM_IN_RUN;
  
  //initialise the mutex
  rc = pthread_mutex_init(&reader->mutex, NULL);
  
  //check for errors
  if(rc) {
    return ERR_UNKNOWN;
  } 
  
  //create and start the input thread
  rc = pthread_create(&reader->thread, NULL, sim_in_loop, reader);
    
  //check for errors
  if(rc) {
    return ERR_UNKNOWN;
  }  
  
  return ERR_NONE;
}

//halt and destory an input reader
void sim_in_destroy(sim_Reader *reader) {
  	
  //tell the input thread to quit
  pthread_mutex_lock(&reader->mutex);
  reader->mode = SIM_IN_QUIT;
  pthread_mutex_unlock(&reader->mutex);
  
  //wait for the thread to finish
  pthread_join(reader->thread, NULL);
  
  //destroy the mutex
  pthread_mutex_destroy(&reader->mutex);
}

//get the last input from the reader and store it in the buffer
void sim_in_read(sim_Reader *reader, int *buffer) {
	
  //lock the struct
  pthread_mutex_lock(&reader->mutex);
  
  //copy the buffer contents
  *buffer = reader->buffer;
  
  //clear the buffer
  reader->buffer = 0;
  
  //unlock the struct
  pthread_mutex_unlock(&reader->mutex);
}

//main loop of sim_reader
void *sim_in_loop(void *reader_pointer) {
  sim_Reader *reader;
  char command[SIM_IN_BUFF_SIZE];
  int input, running;
	
  //get reader pointer from void pointer
  reader = (sim_Reader *) reader_pointer;

  printf("eBuddy text interface\n");
  printf("type h for help\n");
  
  running = 1;
  
  while(running) {
  	
  	//get input from user
  	printf(">> ");
    
    //scanf is unsafe but this module is only for testing
    scanf("%s", command);
    
    input = sim_get_input(command);
      
    //display message for unknown command
    if(input == SIM_IN_UNKNOWN) {
      printf("command '%s' not recognised\n", command);
      printf("type h for help\n");
    }
      
    //print out help message
    else if(input == SIM_IN_HELP) {
      printf("=== commands ===\n");
      printf("n(uts)\t\t- feed nuts and bolts\n");
      printf("b(attery)\t- feed battery\n");
      printf("o(il)\t\t- feed oil\n");
      printf("f(orce)\t\t- force sensor\n");	
      printf("q(uit)\t\t- exit the program\n");
      printf("h(elp)\t\t- print out this message\n");
    }
      
    //store commands in the buffer
    else {
      	
   	  //lock the struct
      pthread_mutex_lock(&reader->mutex);
  
      //store the input in the buffer if it is empty
      if(!reader->buffer) {
        reader->buffer = input;
      }
      
      //unlock the struct
      pthread_mutex_unlock(&reader->mutex);
      
      //sleep for a second to allow command to be processed
      sleep(1);
    }
    
    //check mode for quit signal
    pthread_mutex_lock(&reader->mutex);
    
    if(reader->mode == SIM_IN_QUIT) {
      running = 0;
    }
    
    pthread_mutex_unlock(&reader->mutex);
  }

  //exit thread when loop ends
  printf("Console closed\n");
  
  return NULL;
}

//map an command string to an input number
//uses only the first letter of the command
int sim_get_input(char *command) {
  switch(*command) {
  	
  	//nuts and bolts
    case 'n':
      return INPT_BOLTS;
      
    //battery
    case 'b':
      return INPT_BATTERY;
      
    //oil can
    case 'o':
      return INPT_OIL;
      
    //force sensor
    case 'f':
      return INPT_FORCE;
      
    //power button
    case 'q':
      return INPT_POWER;
  	
  	//help
    case 'h':
      return SIM_IN_HELP;
    
    //unknown command  
    default:
      return SIM_IN_UNKNOWN;
  }
}
