#include "phidget.h"

int ph_init(const char *config, Phhandle *handle)
{
	ph_servo_init(handle->servohandle);
	ph_RFID_rfid_init(handle->RFIDhandle);
	ph_kit_init(handle->IFKhandle);
	ph_LCD_init(handle->LCDhandle);
	
	return 0;
}

int ph_destruct(Phhandle handle)
{
	ph_servo_close(handle->servohandle);
	ph_RFID_close(handle->RFIDhandle);
	ph_lcd_close(handle->LCDhandle);
	ph_kit_close((CPhidgetHandle)handle->IFKhandle);
	return 0;
}

/*Servo Header*/


int ph_servo_init(CPhidgetAdvancedServoHandle *servohandle)
{
	double minAccel, maxVel;
	int servo_wait_result;
	const char *servo_attach_error;
	CPhidgetAdvancedServoHandle servo = 0;
	CPhidgetAdvancedServo_create(&servo);
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)servo, ph_servo_AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)servo, ph_servo_DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)servo, ph_servo_ErrorHandler, NULL);

	CPhidget_open((CPhidgetHandle)servo, -1);

	if((servo_wait_result = CPhidget_waitForAttachment((CPhidgetHandle)servo, 10000)))
	{
		CPhidget_getErrorDescription(servo_wait_result, &servo_attach_error);
		printf("Error eBuddy servo not connected: %s\n", servo_attach_error);
	}

	CPhidgetAdvancedServo_getAccelerationMax(servo, 0, &minAccel);
	CPhidgetAdvancedServo_getVelocityMax(servo, 0, &maxVel);
	servohandle = &servo;
	return 0;


}

int ph_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p)
{
    ph_servo_close(phidget_servo);
    printf("eBuddy servo detatched\n");

	return 0;
}

int ph_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p)
{
    ph_get_servo_handle();
    printf("eBuddy servo attached\n");

	return 0;
}

int ph_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p, int ErrorCode, const char *Description)
{

    printf("eBuddy servo error: %d %s\n", ErrorCode, Description);

	return 0;
}

int ph_servo_close(CPhidgetHandle servo)
{
	CPhidget_close((CPhidgethandle) phidget_servo);
	CPhidget_delete((CPhidgethandle) phidget_servo);

	return 0;
}

/*RFID*/

int ph_RFID_rfid_init(CPhidgetRFIDHandle RFIDhandle)
{
int static result;
const char *err;
CPhidgetRFIDHandle rfid =0;
CPhidgetRFID_create(&rfid);

CPhidget_set_OnAttach_Handler((CPhidgetHandle)rfid, ph_RFID_AttachHandler, NULL);
CPhidget_set_OnDetach_Handler((CPhidgetHandle)rfid, ph_RFID_DetachHandler, NULL);
CPhidget_set_OnError_Handler((CPhidgetHandle)rfid, ph_RFID_ErrorHandler, NULL);


CPhidgetRFID_set_OnTag_Handler(rfid, in_RFID_TagHandler, NULL);
CPhidgetRFID_set_OnTagLost_Handler(rfid, in_RFID_TagLostHandler, NULL);

CPhidget_open((CPhidgetHandle)rfid, -1);

//get the program to wait for an RFID device to be attached
if((result = CPhidget_waitForAttachment((CPhidgetHandle)rfid, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Error ebuddy RFID not connected: %s\n", err);
		ph_RFID_closerfid();
		//exit (1);
	}
CPhidgetRFID_setAntennaOn(rfid, 1);
RFIDhandle = &RFID;
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

int ph_RFID_ErrorHandler(CPhidgetHandle RFID, void *userptr, int ErrorCode, const char *unknown)
{
	//printf("eBuddy RFID error %d - %s\n", ErrorCode, unknown);
	return 0;
}



/*Interface Kit*/

int ph_kit_init(CPhidgetInterfaceKitHandle *IFKhandle)
{
	int result;
	const char *err;
	CPhidgetInterfaceKitHandle ifKit = 0;
	CPhidgetInterfaceKit_create(&ifKit);

	CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifKit, ph_kit_AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifKit, ph_kit_DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)ifKit, ph_kit_ErrorHandler, NULL);


	CPhidgetInterfaceKit_set_OnInputChange_Handler (ifKit, in_kit_InputChangeHandler, NULL);


	CPhidgetInterfaceKit_set_OnSensorChange_Handler (ifKit, in_kit_SensorChangeHandler, NULL);


	CPhidget_open((CPhidgetHandle)ifKit, -1);

	if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifKit, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("eBuddy interface kit not connected: %s\n", err);
		ph_kit_closekit();
		//exit(1);
	}

	IFKhandle = &ifkit;
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
        gs_eyesoff((CPhidgetInterfaceKitHandle)IFK);

	printf("ebuddy interface kit detached!\n");

	return 0;
}

int ph_kit_ErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown)
{
	//printf("ebuddy interface kit Error handled. %d - %s", ErrorCode, unknown);
	return 0;
}





//LCD

int ph_lcd_init(CPhidgetTextLCDHandle *LCDhandle)
{
        int result;
	const char *err;
        //Declare an TextLCD handle
        CPhidgetTextLCDHandle txt_lcd = 0;

	//create the TextLCD object
	CPhidgetTextLCD_create(&txt_lcd);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)txt_lcd, ph_lcd_AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)txt_lcd, ph_lcd_DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)txt_lcd, ph_lcd_ErrorHandler, NULL);

	//open the TextLCD for device connections
	CPhidget_open((CPhidgetHandle)txt_lcd, -1);

	//get the program to wait for an TextLCD device to be attached
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)txt_lcd, 5000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}
        CPhidgetTextLCD_setContrast (txt_lcd, 100);
	LCDhandle = &txt_lcd;
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

int ph_lcd_ErrorHandler(CPhidgetHandle TXT, void *userptr, int ErrorCode, const char *Description)
{
	printf("Error handled. %d - %s\n", ErrorCode, Description);
	return 0;
}

int ph_lcd_close(CPhidgetTextLCDHandle lcd)
{
	CPhidget_close((CPhidgetHandle)lcd);
	CPhidget_delete((CPhidgetHandle)lcd);
}

// end of LCD code
