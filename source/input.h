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

//number of inputs excluding inpt_none
#define IN_NUM_INPUTS 11

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



/*input initialisation*/
int in_input_init(ph_handle *handle);
int in_RFID_init(CPhidgetRFIDHandle RFID);
int in_kit_input_init(CPhidgetInterfaceKitHandle ifKit);
/* RFID input header*/
int in_RFID_TagHandler(CPhidgetRFIDHandle RFID, void *usrptr, unsigned char *TagVal);
int in_RFID_TagLostHandler(CPhidgetRFIDHandle RFID, void *usrptr, unsigned char *TagVal);
void in_RFID_savetag(int tagv);
/*Interface Kit input header*/
int in_kit_InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State);
int in_kit_SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value);

void in_kit_laugh(int sindex, int svalue);
/*in_input_type functions header*/
int in_get_input(void);
void in_set_input(int input);

#endif
