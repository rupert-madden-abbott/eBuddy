#include "phidget.h"
#include "main.h"
#include "gestures.h"


int ph_initialise(void)
{
    ph_get_servo_handle();

    ph_get_RFID_handle();	

    return 0;
}

int ph_destruct(void)
{
    ph_RFID_closerfid();
    return 0;
}


CPhidgetAdvancedServoHandle ph_get_servo_handle(void)
{
   static int servo_initialised = 0;
   static CPhidgetAdvancedServoHandle servo;
   if(servo_initialised == 0){
       servo_initialised = 1;
       servo = ph_servo_initialise();


   }
   return servo;

}



CPhidgetAdvancedServoHandle ph_servo_initialise(void)
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



	gs_set_pos(servo);




	return servo;



}

int ph_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p)
{

    printf("eBuddy servo detatched\n");

	return 0;
}

int ph_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p)
{

    printf("eBuddy servo attached\n");

	return 0;
}

int ph_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p, int ErrorCode, const char *Description)
{

    printf("eBuddy servo error: %d %s\n", ErrorCode, Description);

	return 0;
}

/*RFID*/

CPhidgetRFIDHandle ph_get_RFID_handle (void)
{
	static int RFID_initialised=0;
	static CPhidgetRFIDHandle rfid =0;
	if(RFID_initialised ==0)
		{
			RFID_initialised=1;
			rfid=ph_RFID_openrfid();
			
		}
	return rfid;
}

CPhidgetRFIDHandle ph_RFID_openrfid(void)
{
int static result;
const char *err;
CPhidgetRFIDHandle rfid =0;
CPhidgetRFID_create(&rfid);

CPhidget_set_OnAttach_Handler((CPhidgetHandle)rfid, ph_RFID_AttachHandler, NULL);
CPhidget_set_OnDetach_Handler((CPhidgetHandle)rfid, ph_RFID_DetachHandler, NULL);
CPhidget_set_OnError_Handler((CPhidgetHandle)rfid, ph_RFID_ErrorHandler, NULL);

CPhidgetRFID_set_OnOutputChange_Handler(rfid, ph_RFID_OutputChangeHandler, NULL);

CPhidgetRFID_set_OnTag_Handler(rfid, ph_RFID_TagHandler, NULL);
CPhidgetRFID_set_OnTagLost_Handler(rfid, ph_RFID_TagLostHandler, NULL);

CPhidget_open((CPhidgetHandle)rfid, -1);

//get the program to wait for an RFID device to be attached
if((result = CPhidget_waitForAttachment((CPhidgetHandle)rfid, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Error ebuddy RFID not connected: %s\n", err);
		ph_RFID_closerfid();
		exit (1);
	}
CPhidgetRFID_setAntennaOn(rfid, 1);
return rfid;
}

void ph_RFID_closerfid(void)
{
CPhidgetRFIDHandle rfid;
rfid=ph_get_RFID_handle();
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
	printf("ebuddy RFID error %d - %s\n", ErrorCode, unknown);
	return 0;
}


int ph_RFID_TagHandler(CPhidgetRFIDHandle RFID, void *usrptr, unsigned char *TagVal)
{
	//turn on the Onboard LED
	CPhidgetRFID_setLEDOn(RFID, 1);

	
	printf("Tag Read: %02x%02x%02x%02x%02x\n", TagVal[0], TagVal[1], TagVal[2], TagVal[3], TagVal[4]);
	ph_RFID_savetag(TagVal[0]+TagVal[1]+TagVal[2]+TagVal[3]+TagVal[4]);
	return 0;
}

int ph_RFID_TagLostHandler(CPhidgetRFIDHandle RFID, void *usrptr, unsigned char *TagVal)
{
	//turn off the Onboard LED
	CPhidgetRFID_setLEDOn(RFID, 0);

	printf("Tag Lost: %02x%02x%02x%02x%02x\n", TagVal[0], TagVal[1], TagVal[2], TagVal[3], TagVal[4]);
	return 0;
}

int ph_RFID_OutputChangeHandler(CPhidgetRFIDHandle RFID, void *usrptr, int Index, int State)
{
	if(Index == 0 || Index == 1)
	{
		//printf("Output: %d > State: %d\n", Index, State);
	}
	return 0;
}

void ph_RFID_food(void)
{
	printf("I am eating\n");
}
void ph_RFID_drink(void)
{
	printf("I am drinking\n");
}
void ph_RFID_savetag(int tagv)
{

if(tagv==386)
	{
		ph_RFID_food();
	}
if(tagv==601)
	{
		ph_RFID_drink();
	}

}



