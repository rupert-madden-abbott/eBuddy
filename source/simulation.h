#ifndef SIM_H
#define SIM_H

#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

#include "utility.h"
#include "input.h"

//pause time between commands
#define SIM_IN_PAUSE 2

//size of the input buffer
#define SIM_IN_BUFF_SIZE 20

//input numbers used internally
#define SIM_IN_UNKNOWN -1
#define SIM_IN_HELP -2

//modes used by the sim_in_reader
typedef enum sim_in_mode {
  SIM_IN_RUN,
  SIM_IN_QUIT	
} sim_in_mode;

//a sim input reads commands from a file
//and translates them into eBuddy inputs
typedef struct sim_reader {
  pthread_t thread;			//the thread the input loop runs in
  pthread_mutex_t mutex;	//lock for mode and buffer
  int buffer;				//the last input from the user
  int mode;					//controls the behaviour of the input thread
} sim_Reader;


//create and start a new input reader that reads from the given file
int sim_in_init(sim_Reader *reader);

//halt and destory an input reader
void sim_in_destroy(sim_Reader *reader);

//get the last input from the reader and store it in the buffer
void sim_in_read(sim_Reader *reader, int *buffer);

//main loop of sim_reader
void *sim_in_loop(void *reader);

//map an command string to an input number
//uses only the first letter of the command
int sim_get_input(char *command);

#endif
