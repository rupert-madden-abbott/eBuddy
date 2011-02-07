/**
 *@file phidget.h
 *@author Andrew Barber (servos), Andrew Pickin (LCD) & Ahmed Sarwat (input).
 *This modules contains all common phidget code including initialisation and
 *destruction.
 */


#ifndef PHIDGET2_H
#define PHIDGET2_H

#include <stdio.h>
#include <phidget21.h>

/**
 *@struct ph_handle
 *contains CPhidgetHandles for each type of phidget used
 */
typedef struct ph_handle {
  CPhidgetAdvancedServoHandle servohandle;
  CPhidgetRFIDHandle          RFIDhandle;
  CPhidgetInterfaceKitHandle  IFKhandle;
  CPhidgetTextLCDHandle       LCDhandle;
} ph_handle;

/**
 *Function calls each phidget initialiser in turn
 *@arg const char *config Allows any config to be passed in.
 *@arg ph_handle *handle Struct points to the handles for each phidget.
 */
int ph_init(const char *config, ph_handle *handle);

/**
 *Function calls each phidget destructor in turn
 *@arg ph_handle *handle Struct points to the handles for each phidget.
 */
int ph_destruct(ph_handle *handle);

///{
/**
 *Phidget initialisers
 *Functions for initilising phidgets inclduing setting callback functions and 
 *creating handles.
 *@arg ph_handle *handle Struct points to the handles for each phidget.
 *@return int phidget library function call return code.
 */
int ph_servo_init(ph_handle *handle);
int ph_RFID_rfid_init(ph_handle *handle);
int ph_kit_init(ph_handle *handle);
int ph_lcd_init(ph_handle *handle);
///}

///{
/**
 *Phidget Destructors
 *Functions close phidgets and destroy handles
 *@arg ph_handle *handle Struct points to the handles for each phidget.
 */
int ph_servo_close(CPhidgetAdvancedServoHandle servo);
void ph_RFID_close(CPhidgetRFIDHandle rfid);
void ph_kit_close(CPhidgetInterfaceKitHandle IFK);
int ph_lcd_close(CPhidgetTextLCDHandle lcd);
///}

///{
/**
 *Phidget AttachHandlers
 *Callback functions called on phidget attach.
 *@arg ph_handle *handle Struct points to the handles for each phidget.
 */
int ph_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p);
int ph_RFID_AttachHandler(CPhidgetHandle RFID, void *userptr);
int ph_kit_AttachHandler(CPhidgetHandle IFK, void *userptr);
int ph_lcd_AttachHandler(CPhidgetHandle TXT, void *userptr);
///}


///{
/**
 *Phidget DetachHandlers
 *Callback functions called on phidget detach
 *@arg ph_handle *handle Struct points to the handles for each phidget. 
 */
int ph_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p);
int ph_RFID_DetachHandler(CPhidgetHandle RFID, void *userptr);
int ph_kit_DetachHandler(CPhidgetHandle IFK, void *userptr);
int ph_lcd_DetachHandler(CPhidgetHandle TXT, void *userptr);
///}

///{
/**
 *Phdget ErrorHandlers
 *Callback functions called on phidget error
 *@arg ph_handle *handle Struct points to the handles for each phidget.
 */
int ph_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p, int ErrorCode,
                                         const char *Description);
int ph_RFID_ErrorHandler(CPhidgetHandle RFID, void *userptr, int ErrorCode,
                                         const char *unknown);
int ph_kit_ErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode,
                                         const char *unknown);
int ph_lcd_ErrorHandler(CPhidgetHandle TXT, void *userptr, int ErrorCode, 
                                         const char *Description);
///}


#endif
