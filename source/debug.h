#ifndef db_H
#define db_H

#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "utility.h"
#include "input.h"
#include "emotion.h"
#include "gesture_interface.h"
#include "main.h"
#include "notify.h"
#include "queue.h"

//size between steps when setting or updating emotions
#define DB_EM_STEP 10

//size of the buffer used to store generated text
#define DB_BUFF_SIZE 32

//menu item number used to exit db
#define DB_EXIT -1

//db mode entry point
int db_main(em_State *emotions, qu_queue *notifications, ph_handle *phhandle);

//emotion menu allows user to get and set emotion levels
int db_emotions(em_State *emotions, qu_queue *notifications, ph_handle *phhandle);

//print event stream to lcd screen
int db_events(em_State *emotions, qu_queue *notifications, ph_handle *phhandle);

//manually change modes
int db_modes(em_State *emotions, qu_queue *notifications, ph_handle *phhandle);

//display a menu on the lcd screen allowing the user to choose between
//item's. the function returns the item number or db_none if the operation
//is canceled
int db_menu(const char **items, int num_items, ph_handle *phhandle);

//get a number from the user between min and max with
//steps of the given size
int db_input(int min, int max, int step, ph_handle *phhandle);


#endif
