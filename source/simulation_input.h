/**
 * @file simulation_input.h
 * @author Rowan Saundry
 *
 * The simulation reader is a threaded command line interface used by
 * the simulation module to emulate the input library.
 */

#ifndef SIM_H
#define SIM_H

#include <pthread.h>

#include "input.h"

//The length of the pause between commands
#define SMI_PAUSE 2

//The size of the input buffer
#define SMI_BUFF_SIZE 20

//Input numbers used internally
#define SMI_UNKNOWN -1
#define SMI_HELP -2

//Modes used by the smi_reader
typedef enum smi_mode {
  SMI_RUN,
  SMI_QUIT
} smi_mode;

/**
 * The smi reader is the stuct used to represent and communicate with the
 * input reader and contains the thread, lock, buffer and mode variable.
 */
typedef struct smi_reader {
  pthread_t thread;         //The thread the input loop runs in
  pthread_mutex_t mutex;    //Lock for mode and buffer
  int buffer;               //The last input from the user
  int mode;                 //Controls the behaviour of the input thread
} smi_Reader;


/**
 * Creates a new input reader and returns a pointer to it. Returns NULL
 * if malloc fails or if the pthread or mutex cannot be initialised.
 *
 * @return A new input reader or NULL on error
 */
smi_Reader *smi_init(void);

/**
 * Halts the input reader thread and frees and objects used by it
 * including the struct itself.
 *
 * @arg reader The reader object to destroy
 */
void smi_destroy(smi_Reader *reader);

/**
 * Get the last input from the reader and store it in the buffer provided.
 * Inputs numbers are the same as those used by the input module.
 *
 * @arg reader The input reader to read from
 * @arg buffer The buffer to store the input in
 */
void smi_read(smi_Reader *reader, in_input_type *buffer);

/**
 * The main loop of the input readers internal thread. Loops continuously
 * reading commands and storing them in the input buffer until mode is set
 * to SMI_QUIT. This is an internal function and should not be called from
 * outside the module. The reader stuct is passed as a void pointer because
 * it is sent as an argument to the pthread_create function.
 *
 * @arg reader The reader struct used for configuration and communication
 */
void *smi_loop(void *reader);

/**
 * Internal function used to map command strings to an input number. The
 * mapping is performed using a switch case statement so only the first
 * letter of the command is used to decide which input was requested. 
 * Returns SMI_HELP if the help command was entered and SMI_UNKNOWN if the
 * command was not recognised.
 *
 * @arg command The command string to map
 * @return The input number the command maps to
 */
in_input_type smi_get_input(char *command);

#endif
