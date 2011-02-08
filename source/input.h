/**
 * @file   input.h
 * @author Ahmed Sarwat
 *
 * This module manages different inputs of phidgets.
 */
#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <time.h>

#include <phidget21.h>
#include "phidget.h"

#define IN_PRESS 400
#define IN_DISC 386
#define IN_CARDONE 601
#define IN_CARDTWO 558
#define IN_KEYFOBONE 252
#define IN_KEYFOBTWO 375
#define IN_TOUCHONE 1
#define IN_TOUCHTWO 2
#define IN_FORCE 0
#define IN_LIGHT 3
#define IN_POWER_ON 0
#define IN_POWER_OFF 1

//number of inputs excluding inpt_none
#define IN_NUM_INPUTS 11

/**
 * The input types
 * 
 * @enum INPT_NONE indicates that there is no input.
 * @enum INPT_BOLTS indicates the BOLTS rfid tag has been detected.
 * @enum INPT_OIL indicates the OIL rfid tag has been detected.
 * @enum INPT_BATTERY indicates the BATTERY rfid tag has been detected.
 * @enum INPT_FORCE indicates that the force  sensor is pressed.
 * @enum INPT_DARK indicates that there is no light.
 * @enum INPT_RIGHT_HAND indicates that the right hand is touched.
 * @enum INPT_LEFT_HAND indicates that the left hand is touched.
 * @enum INPT_POWER_ON indicates that the on button is pressed.
 * @enum INPT_POWER_OFF indicates that the off button is pressed.
 * @enum INPT_DEMO indicates that the demo rfid tag has been detected.
 * @enum INPT_DEBUG indicates that the debug rfid tag has been detected.
 * 
 */

enum in_input_type{
INPT_NONE = 0,
INPT_BOLTS = 1,
INPT_OIL = 2,
INPT_BATTERY = 3,
INPT_FORCE = 4,
INPT_DARK = 5,
INPT_RIGHT_HAND = 6,
INPT_LEFT_HAND = 7,
INPT_POWER_ON = 8,
INPT_POWER_OFF = 9,
INPT_DEMO = 10,
INPT_DEBUG = 11
};
typedef enum in_input_type in_input_type;



//input initialisation
int in_input_init(ph_handle *handle);

/**
 * Initialize RFID phidget handle
 * 
 * @param  RFID the RFID phidget handel
 * 
 */
int in_RFID_init(CPhidgetRFIDHandle RFID);

/**
 * Initialize Interface Kit phidget handle
 * 
 * @param  ifKit the Interface Kit phidget handel
 * 
 */
int in_kit_input_init(CPhidgetInterfaceKitHandle ifKit);
// RFID input header


/**
 * reads any tag whenever it a tag is detected
 * 
 * @param  RFID the RFID phidget handel
 * 
 */
int in_RFID_TagHandler(CPhidgetRFIDHandle RFID, void *usrptr, 
                       unsigned char *TagVal);

/**
 * shows any tag after when it is no longer detected
 * 
 * @param  RFID the RFID phidget handel
 * 
 */
int in_RFID_TagLostHandler(CPhidgetRFIDHandle RFID, void *usrptr, 
                           unsigned char *TagVal);

/**
 * whenever a tag is detected , this function is called and hold the tag value
 * to analyze and specify the input type
 *
 * @param  tagv holds the read tag value
 * 
 */
void in_RFID_savetag(int tagv);
//Interface Kit input header

/**
 * senses any change in the input
 * 
 * @param  IFK the Interdace Kit phidget handel
 * @param  Index the input index
 * @param  State the input value
 */
int in_kit_InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, 
                              int Index, int State);

/**
 * senses any change in the sensors
 * 
 * @param  IFK the Interdace Kit phidget handel
 * @param  Index the input index
 * @param  Value the input value
 */
int in_kit_SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, 
                               int Index, int Value);

/**
 * whenever a sensor change is detected , this function is called and hold the 
 * sensor index and its value to analyze and specify the input type
 * 
 * @param  sindex the sensor index
 * @param  svalu the sensor value
 */
void in_kit_save(int sindex, int svalue);

/**
 * whenever an input state change is detected , this function is called and  
 * hold input index and its value to analyze and specify the input type
 * 
 * @param  in_index the input index
 * @param  in_value the in_sensor value
 */
void ph_kit_inputs(int in_index, int in_value);
//in_input_type functions header

/**
 * whenever it is called,it will return the last input
 * 
 * @return last input value
 * 
 */
int in_get_input(void);

/**
 * whenever an input is detected , this function will be used to set the input
 * but the input must be used before setting it with a new input type.
 * 
 * @param  input the last input type
 * 
 */
void in_set_input(int input);

#endif
