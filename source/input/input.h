enum in_input_type{
battery = 386,
oil = 601,
nuts_and_bolts = 558
nothing = 0;
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
