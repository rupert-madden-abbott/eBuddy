#ifndef DEBUG_H
#define DEBUG_H

#include "emotion.h"
#include "notify.h"

//size of the buffer used to store number strings
//this value is large enough to store -2^32
#define DEBUG_NUM_STR_LEN 12

//menu item number used to exit menu
#define DEBUG_BACK -1

//menu item number used to exit debug
#define DEBUG_EXIT -2

//debug mode entry point
int debug_main(em_State *emotions, qu_queue *notifications);

//emotion menu allows user to get and set emotion levels
int debug_emotions(em_State *emotions, qu_queue *notifications);

//print event stream to lcd screen
int debug_events(em_State *emotions, qu_queue *notifications);

//display a menu on the lcd screen allowing the user to choose between
//item's. the function returns the item number or debug_none if the operation
//is canceled
int debug_menu(const char **items, int num_items);

//get a number from the user between min and max with
//steps of the given size
int debug_input(int min, int max, int step);
#endif