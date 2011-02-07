/**
 * @file debug.c
 * @author Rowan Saundry
 *
 * The debug module contains tools useful for developing and testing
 * the eBuddy software. It is implimented as a run mode and interacts
 * with the user using the input sensors and the LCD screen. 
 */

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

//The size between steps when setting or updating emotions
#define DB_EM_STEP 10

//The size of the buffer used to store generated text
#define DB_BUFF_SIZE 32

//The menu item number used to exit debug
#define DB_EXIT -999


/**
 * Contains the debug main menu. Simply calls db_menu with a list of
 * debug tools and uses a switch case statement to call the correct sub menu
 * or exits if BD_EXIT is recieved.
 *
 * @arg emotions The eBuddys emotion state
 * @arg notifications The notification queue
 * @arg phhandle The handle used to access the phidgets
 * @return A ut_error_code. Passes back errors from child modes.
 */
ut_ErrorCode db_main(em_State *emotions, qu_queue *notifications,
                     ph_handle *phhandle);

/**
 * Contains the emotion tool. Allows user to get set or update any emotion
 * level. Emotion names are read from the emotion module. Does not allow
 * values to be set out of bounds. Menu step size can be controlled with
 * DB_EM_STEP.
 *
 * @arg emotions The eBuddys emotion state
 * @arg notifications The notification queue
 * @arg phhandle The handle used to access the phidgets
 * @return A ut_error_code. Passes back errors from child modes.
 */
ut_ErrorCode db_emotions(em_State *emotions, qu_queue *notifications,
                         ph_handle *phhandle);

/**
 * Contains the event tool. Allows user to view raw notification, emotion
 * and input events. The event type is printed to the LCD screen followed by
 * its arguments. Returns when the user presses the power button.
 *
 * @arg emotions The eBuddys emotion state
 * @arg notifications The notification queue
 * @arg phhandle The handle used to access the phidgets
 * @return A ut_error_code. Passes back errors from child modes.
 */
ut_ErrorCode db_events(em_State *emotions, qu_queue *notifications,
                       ph_handle *phhandle);

/**
 * Contains the mode tool. Allows user to manually select a mode by name
 * from a menu.
 *
 * @arg emotions The eBuddys emotion state
 * @arg notifications The notification queue
 * @arg phhandle The handle used to access the phidgets
 * @return A ut_error_code. Passes back errors from child modes.
 */
ut_ErrorCode db_modes(em_State *emotions, qu_queue *notifications,
                      ph_handle *phhandle);

/**
 * Displays a menu on the lcd screen allowing the user to choose between
 * items. The function returns the item number or db_none if the operation
 * is canceled.
 *
 * @arg items A string array of items in the menu
 * @arg num_items the number of items in the array
 * @arg phhandle The handle used to access the phidgets
 * @return The element number of the item selected
 */
int db_menu(const char **items, int num_items, ph_handle *phhandle);

/**
 * Displays a menu on the lcd screen allowing the user to input a number.
 * The function returns the value selected or db_none if the operation
 * is canceled. The default value is exactly half way between min and max.
 *
 * @arg min Minimum value that can be selected
 * @arg max Maximum value that can be selected
 * @arg phhandle The handle used to access the phidgets
 * @arg step The size of the step between values
 * @return The element number of the item selected
 */
int db_input(int min, int max, int step, ph_handle *phhandle);

#endif
