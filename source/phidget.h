#ifndef PHIDGET2_H
#define PHIDGET2_H

#include <stdio.h>

#include <phidget21.h>



typedef struct Phhandle{
CPhidgetAdvancedServoHandle servohandle
CPhidgetRFIDHandle          RFIDhandle
CPhidgetInterfaceKitHandle  IFKhandle
CPhidgetTextLCDHandle       LCDhandle
}

extern int ph_init(const char *config, Phhandle *handle);

extern int ph_destruct(Phhandle handle);

/*Servo Header*/

int ph_servo_init(CPhidgetAdvancedServoHandle *servohandle);
int ph_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p);
int ph_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p);
int ph_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p, int ErrorCode, const char *Description);
int ph_servo_close(CPhidgetHandle servo);

/*RFID Header*/

int ph_RFID_rfid_init(CPhidgetRFIDHandle RFIDhandle)
int ph_RFID_AttachHandler(CPhidgetHandle RFID, void *userptr);
int ph_RFID_DetachHandler(CPhidgetHandle RFID, void *userptr);
int ph_RFID_ErrorHandler(CPhidgetHandle RFID, void *userptr, int ErrorCode, const char *unknown);
CPhidgetRFIDHandle ph_RFID_openrfid(void);
CPhidgetRFIDHandle ph_get_RFID_handle (void);
void ph_RFID_close(CPhidgetRFIDHandle rfid);

/*Interface Kit Header*/
int ph_kit_init(CPhidgetInterfaceKitHandle *IFKhandle)
void ph_kit_close(CPhidgetInterfaceKitHandle IFK)
int ph_kit_AttachHandler(CPhidgetHandle IFK, void *userptr);
int ph_kit_DetachHandler(CPhidgetHandle IFK, void *userptr);
int ph_kit_ErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown);
int ph_lcd_ErrorHandler(CPhidgetHandle TXT, void *userptr, int ErrorCode, const char *Description);
int ph_lcd_DetachHandler(CPhidgetHandle TXT, void *userptr);
int ph_lcd_AttachHandler(CPhidgetHandle TXT, void *userptr);


int ph_lcd_init(CPhidgetTextLCDHandle *LCDhandle);
int ph_lcd_close(CPhidgetTextLCDHandle lcd);
#endif
