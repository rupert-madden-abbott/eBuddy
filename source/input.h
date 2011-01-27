#ifndef INPUT_H
#define INPUT_H
#define IN_PRESS 400
#define IN_DISC 386
#define IN_CARD 601
#define IN_KEYFOB 252


enum in_input_type{
INPT_NONE = 0,
INPT_BATTERY,
INPT_OIL,
INPT_BOLTS,
INPT_FORCE,
INPT_POWER
};
typedef enum in_input_type in_input_type;

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
