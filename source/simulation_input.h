#ifndef SIM_H
#define SIM_H

#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

#include "utility.h"
#include "input.h"

//pause time between commands
#define SMI_PAUSE 2

//size of the input buffer
#define SMI_BUFF_SIZE 20

//input numbers used internally
#define SMI_UNKNOWN -1
#define SMI_HELP -2

//modes used by the smi_reader
typedef enum smi_mode {
  SMI_RUN,
  SMI_QUIT	
} smi_mode;

//a sim input reads commands from a file
//and translates them into eBuddy inputs
typedef struct smi_reader {
  pthread_t thread;			//the thread the input loop runs in
  pthread_mutex_t mutex;	//lock for mode and buffer
  int buffer;				//the last input from the user
  int mode;					//controls the behaviour of the input thread
} smi_Reader;


//create and start a new input reader that reads from the given file
smi_Reader *smi_init(void);

//halt and destory an input reader
void smi_destroy(smi_Reader *reader);

//get the last input from the reader and store it in the buffer
void smi_read(smi_Reader *reader, int *buffer);

//main loop of sim_reader
void *smi_loop(void *reader);

//map an command string to an input number
//uses only the first letter of the command
int smi_get_input(char *command);

#endif
