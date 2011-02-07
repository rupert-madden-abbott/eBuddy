/* the simulation module is used by the simulation
 * wrapper to allow it to emulate the phidgits. */
#include <stdlib.h> 
#include "simulation_input.h"

//create and start a new input reader
smi_Reader *smi_init(void) {
  smi_Reader *reader;
  int rc;
  
  //create a new sim reader
  reader = (smi_Reader *) malloc(sizeof(smi_Reader));
	
  //clear input buffer
  reader->buffer = INPT_NONE;
  
  //set mode to running
  reader->mode = SMI_RUN;
  
  //initialise the mutex
  rc = pthread_mutex_init(&reader->mutex, NULL);
  
  //check for errors
  if(rc) {
  	free(reader);
    return NULL;
  } 
  
  //create and start the input thread
  rc = pthread_create(&reader->thread, NULL, smi_loop, reader);
    
  //check for errors
  if(rc) {
  	free(reader->thread);
  	free(reader);
    return NULL;
  }  
  
  return reader;
}

//halt and destory an input reader
void smi_destroy(smi_Reader *reader) {
  	
  //tell the input thread to quit
  pthread_mutex_lock(&reader->mutex);
  reader->mode = SMI_QUIT;
  pthread_mutex_unlock(&reader->mutex);
  
  //wait for the thread to finish
  pthread_join(reader->thread, NULL);
  
  //destroy the mutex
  pthread_mutex_destroy(&reader->mutex);
  
  //free the struct
  free(reader);
}

//get the last input from the reader and store it in the buffer
void smi_read(smi_Reader *reader, int *buffer) {
	
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
void *smi_loop(void *reader_pointer) {
  smi_Reader *reader;
  char command[SMI_BUFF_SIZE];
  int input, running;
	
  //get reader pointer from void pointer
  reader = (smi_Reader *) reader_pointer;

  printf("eBuddy text interface\n");
  printf("type h for help\n");
  
  //send power on signal
  pthread_mutex_lock(&reader->mutex);
  reader->buffer = INPT_POWER_ON;
  pthread_mutex_unlock(&reader->mutex);
  
  //loop until quit signal
  running = 1;
  
  while(running) {
  	
  	//get input from user
  	printf(">> ");
    
    //scanf is unsafe but this module is only for testing
    scanf("%s", command);
    
    input = smi_get_input(command);
      
    //display message for unknown command
    if(input == SMI_UNKNOWN) {
      printf("command '%s' not recognised\n", command);
      printf("type h for help\n");
    }
      
    //print out help message
    else if(input == SMI_HELP) {
      printf("=== commands ===\n");
      printf("n(uts)\t\t- nuts and bolts key\n");
      printf("b(attery)\t- battery key\n");
      printf("o(il)\t\t- oil key\n");
      printf("f(orce)\t\t- force sensor\n");
      printf("s(leep)\t\t- sleep sensor\n");
      printf("l(eft)\t\t- left hand\n");
      printf("r(ight)\t\t- right hand\n");
      printf("(dem)m(o)\t- demo key\n");
      printf("d(ebug)\t\t- debug key\n");	
      printf("q(uit)\t\t- send shutdown signal\n");
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
      
      //sleep for a while to allow command to be processed
      sleep(SMI_PAUSE);
    }
    
    //check mode for quit signal
    pthread_mutex_lock(&reader->mutex);
    
    if(reader->mode == SMI_QUIT) {
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
int smi_get_input(char *command) {
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
      
    //sleep sensor
    case 's':
      return INPT_DARK;
    
    //press left hand
    case 'l':
      return INPT_LEFT_HAND;
    
    //press right hand
    case 'r':
      return INPT_RIGHT_HAND;
      
    //power button
    case 'q':
      return INPT_POWER_OFF;
    
    //demo key
    case 'm':
      return INPT_DEMO;
     
    //debug key 
    case 'd':
      return INPT_DEBUG;
  	
  	//help
    case 'h':
      return SMI_HELP;
    
    //unknown command  
    default:
      return SMI_UNKNOWN;
  }
}
