#include <phidget21.h>


extern int ph_initialise(void);
extern CPhidgetAdvancedServoHandle ph_get_servo_handle(void);
CPhidgetAdvancedServoHandle ph_servo_initialise(void);
int ph_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p);
int ph_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p);
int ph_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p, int ErrorCode, const char *Description);

/*RFID Header*/
void ph_RFID_food(void);
void ph_RFID_drink(void);
void ph_RFID_savetag(int tagv);
int ph_RFID_AttachHandler(CPhidgetHandle RFID, void *userptr);
int ph_RFID_DetachHandler(CPhidgetHandle RFID, void *userptr);
int ph_RFID_ErrorHandler(CPhidgetHandle RFID, void *userptr, int ErrorCode, const char *unknown);
int ph_RFID_TagHandler(CPhidgetRFIDHandle RFID, void *usrptr, unsigned char *TagVal);
int ph_RFID_TagLostHandler(CPhidgetRFIDHandle RFID, void *usrptr, unsigned char *TagVal);
int ph_RFID_OutputChangeHandler(CPhidgetRFIDHandle RFID, void *usrptr, int Index, int State);
CPhidgetRFIDHandle ph_RFID_openrfid(void);
CPhidgetRFIDHandle ph_get_RFID_handle (void);
void ph_RFID_closerfid(void);



