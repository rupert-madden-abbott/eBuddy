#include "phidget.h"


int ph_init(const char *config, ph_handle *handle)
{
    ph_servo_init(handle);
    ph_RFID_rfid_init(handle);
    ph_kit_init(handle);
    ph_lcd_init(handle);
	
    return 0;
}

int ph_destruct(ph_handle *handle)
{
    ph_servo_close(handle->servohandle);
    ph_RFID_close(handle->RFIDhandle);
    ph_lcd_close(handle->LCDhandle);
    ph_kit_close(handle->IFKhandle);
    return 0;
}

/*Servo Header*/


int ph_servo_init(ph_handle *handle)
{
    double minAccel, maxVel;
    int servo_wait_result;
    const char *servo_attach_error;
    handle->servohandle = 0;
    CPhidgetAdvancedServo_create(&handle->servohandle);
    CPhidget_set_OnAttach_Handler((CPhidgetHandle)handle->servohandle,
                                  ph_servo_AttachHandler, NULL);
    CPhidget_set_OnDetach_Handler((CPhidgetHandle)handle->servohandle,
                                  ph_servo_DetachHandler, NULL);
    CPhidget_set_OnError_Handler((CPhidgetHandle)handle->servohandle,
                                 ph_servo_ErrorHandler, NULL);

    CPhidget_open((CPhidgetHandle)handle->servohandle, -1);

    if((servo_wait_result = CPhidget_waitForAttachment((CPhidgetHandle)handle->
                                                       servohandle, 10000)))
    {
        CPhidget_getErrorDescription(servo_wait_result, &servo_attach_error);
        printf("Error eBuddy servo not connected: %s\n", servo_attach_error);
    }

    CPhidgetAdvancedServo_getAccelerationMax(handle->servohandle, 0, &minAccel);
    CPhidgetAdvancedServo_getVelocityMax(handle->servohandle, 0, &maxVel);
    return 0;


}

int ph_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p)
{
    ph_servo_close((CPhidgetAdvancedServoHandle) phidget_servo);
    printf("eBuddy servo detatched\n");

	return 0;
}

int ph_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p)
{
    
    printf("eBuddy servo attached\n");

	return 0;
}

int ph_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p,
                          int ErrorCode, const char *Description)
{

    printf("eBuddy servo error: %d %s\n", ErrorCode, Description);

	return 0;
}

int ph_servo_close(CPhidgetAdvancedServoHandle servo)
{
	CPhidget_close((CPhidgetHandle) servo);
	CPhidget_delete((CPhidgetHandle) servo);

	return 0;
}

/*RFID*/

int ph_RFID_rfid_init(ph_handle *handle)
{
int static result;
const char *err;
handle->RFIDhandle =0;
CPhidgetRFID_create(&handle->RFIDhandle);

CPhidget_set_OnAttach_Handler((CPhidgetHandle)handle->RFIDhandle, 
                              ph_RFID_AttachHandler, NULL);
CPhidget_set_OnDetach_Handler((CPhidgetHandle)handle->RFIDhandle,
                              ph_RFID_DetachHandler, NULL);
CPhidget_set_OnError_Handler((CPhidgetHandle)handle->RFIDhandle, 
                              ph_RFID_ErrorHandler, NULL);

CPhidget_open((CPhidgetHandle)handle->RFIDhandle, -1);

//CPhidget_open((CPhidgetHandle)rfid, -1);

//get the program to wait for an RFID device to be attached
if((result = CPhidget_waitForAttachment((CPhidgetHandle)handle->RFIDhandle, 
                                        10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Error ebuddy RFID not connected: %s\n", err);
		ph_RFID_close(handle->RFIDhandle);
		
	}
//CPhidgetRFID_setAntennaOn(rfid, 1);
return 0;
}

void ph_RFID_close(CPhidgetRFIDHandle rfid)
{

CPhidget_close((CPhidgetHandle)rfid);
CPhidget_delete((CPhidgetHandle)rfid);
}


int ph_RFID_AttachHandler(CPhidgetHandle RFID, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (RFID, &name);
	CPhidget_getSerialNumber(RFID, &serialNo);
	printf("ebuddy RFID attached\n");

	return 0;
}

int ph_RFID_DetachHandler(CPhidgetHandle RFID, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (RFID, &name);
	CPhidget_getSerialNumber(RFID, &serialNo);
	printf("ebuddy RFID detached!\n");

	return 0;
}

int ph_RFID_ErrorHandler(CPhidgetHandle RFID, void *userptr, int ErrorCode,
                         const char *unknown)
{
	//printf("eBuddy RFID error %d - %s\n", ErrorCode, unknown);
	return 0;
}



/*Interface Kit*/

int ph_kit_init(ph_handle *handle)
{
	int result;
	const char *err;
	handle->IFKhandle = 0;
	CPhidgetInterfaceKit_create(&handle->IFKhandle);

	CPhidget_set_OnAttach_Handler((CPhidgetHandle)handle->IFKhandle,
                                      ph_kit_AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)handle->IFKhandle,
                                      ph_kit_DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)handle->IFKhandle,
                                     ph_kit_ErrorHandler, NULL);
	CPhidget_open((CPhidgetHandle)handle->IFKhandle, -1);

	if((result = CPhidget_waitForAttachment((CPhidgetHandle)handle->
                                                IFKhandle,10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("eBuddy interface kit not connected: %s\n", err);
		ph_kit_close(handle->IFKhandle);
		//exit(1);
	}

	return 0;
}

void ph_kit_close(CPhidgetInterfaceKitHandle IFK)
{
	CPhidget_close((CPhidgetHandle)IFK);
	CPhidget_delete((CPhidgetHandle)IFK);

}

int ph_kit_AttachHandler(CPhidgetHandle IFK, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName(IFK, &name);
	CPhidget_getSerialNumber(IFK, &serialNo);

	printf("ebuddy interface kit attached!\n");

	return 0;
}

int ph_kit_DetachHandler(CPhidgetHandle IFK, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (IFK, &name);
	CPhidget_getSerialNumber(IFK, &serialNo);

	printf("ebuddy interface kit detached!\n");

	return 0;
}

int ph_kit_ErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode,
                        const char *unknown)
{
//printf("ebuddy interface kit Error handled. %d - %s",ErrorCode, unknown);
	return 0;
}





//LCD

int ph_lcd_init(ph_handle *handle)
{
        int result;
	const char *err;
        //Declare an TextLCD handle
        handle->LCDhandle = 0;

	//create the TextLCD object
	CPhidgetTextLCD_create(&handle->LCDhandle);

//Set the handlers to be run when the device is plugged in or opened from
//software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)handle->LCDhandle,
                                      ph_lcd_AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)handle->LCDhandle,
                                      ph_lcd_DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)handle->LCDhandle,
                                     ph_lcd_ErrorHandler, NULL);

	//open the TextLCD for device connections
	CPhidget_open((CPhidgetHandle)handle->LCDhandle, -1);

	//get the program to wait for an TextLCD device to be attached
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)handle->
                                                LCDhandle, 5000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}
        
        return 0;
}

int ph_lcd_AttachHandler(CPhidgetHandle TXT, void *userptr) {
	printf("ebuddy LCD attached!\n");

	return 0;
}

int ph_lcd_DetachHandler(CPhidgetHandle TXT, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (TXT, &name);
	CPhidget_getSerialNumber(TXT, &serialNo);
	printf("%s %10d detached!\n", name, serialNo);

	return 0;
}

int ph_lcd_ErrorHandler(CPhidgetHandle TXT, void *userptr, int ErrorCode,
                        const char *Description)
{
	printf("Error handled. %d - %s\n", ErrorCode, Description);
	return 0;
}

int ph_lcd_close(CPhidgetTextLCDHandle lcd)
{
	CPhidget_close((CPhidgetHandle)lcd);
	CPhidget_delete((CPhidgetHandle)lcd);
    return 0;
}

// end of LCD code
