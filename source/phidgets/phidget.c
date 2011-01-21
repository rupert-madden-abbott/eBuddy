#include "phidget.h"
#include "main.h"
#include "gestures.h"


extern int ph_init(PhidgetHandle phidgets, const char *config)
{
    int status;
    CPhidgetInterfaceKitHandle ifkit;
    ph_get_servo_handle();

    //ph_get_RFID_handle();

    ifkit = ph_get_kit_handle();
    CPhidget_getDeviceStatus((CPhidgetHandle)ifkit, &status);
    if(status == PHIDGET_ATTACHED){
	gs_eyeson(ifkit);
    }
	
    	

    return 0;
}

int ph_destruct(void)
{
    ph_servo_close((CPhidgetHandle)ph_get_servo_handle());
    ph_RFID_closerfid();

    ph_kit_closekit();
    return 0;
}

/*Servo Header*/
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

int ph_servo_close(CPhidgetHandle phidget_servo)
{
	CPhidget_close(phidget_servo);
	CPhidget_delete(phidget_servo);
	
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
		//exit (1);
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
	printf("eBuddy RFID error %d - %s\n", ErrorCode, unknown);
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


/*Interface Kit*/
CPhidgetInterfaceKitHandle ph_get_kit_handle (void)
{
	static int kit_initialised=0;
	static CPhidgetInterfaceKitHandle ifKit =0;
	if(kit_initialised ==0)
		{
			kit_initialised=1;
			ifKit=ph_kit_openkit();
			
		}
	return ifKit;
}

CPhidgetInterfaceKitHandle ph_kit_openkit(void)
{
	int result;
	const char *err;
	CPhidgetInterfaceKitHandle ifKit = 0;
	CPhidgetInterfaceKit_create(&ifKit);

	CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifKit, ph_kit_AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifKit, ph_kit_DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)ifKit, ph_kit_ErrorHandler, NULL);


	CPhidgetInterfaceKit_set_OnInputChange_Handler (ifKit, ph_kit_InputChangeHandler, NULL);


	CPhidgetInterfaceKit_set_OnSensorChange_Handler (ifKit, ph_kit_SensorChangeHandler, NULL);

	CPhidgetInterfaceKit_set_OnOutputChange_Handler (ifKit, ph_kit_OutputChangeHandler, NULL);

	CPhidget_open((CPhidgetHandle)ifKit, -1);

	if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifKit, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("eBuddy interface kit not connected: %s\n", err);
		ph_kit_closekit();
		//exit(1);
	}

	
return ifKit;
}

void ph_kit_closekit(void)
{
	CPhidgetInterfaceKitHandle ifKit;
	ifKit=ph_get_kit_handle();
	CPhidget_close((CPhidgetHandle)ifKit);
	CPhidget_delete((CPhidgetHandle)ifKit);

}

int ph_kit_AttachHandler(CPhidgetHandle IFK, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName(IFK, &name);
	CPhidget_getSerialNumber(IFK, &serialNo);
	gs_eyeson(IFK);

	printf("ebuddy interface kit attached!\n");

	return 0;
}

int ph_kit_DetachHandler(CPhidgetHandle IFK, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (IFK, &name);
	CPhidget_getSerialNumber(IFK, &serialNo);
        gs_eyesoff(IFK);

	printf("ebuddy interface kit detached!\n");

	return 0;
}

int ph_kit_ErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown)
{
	printf("ebuddy interface kit Error handled. %d - %s", ErrorCode, unknown);
	return 0;
}

int ph_kit_InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
	//printf("Digital Input: %d > State: %d\n", Index, State);
	return 0;
}

int ph_kit_OutputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
	//printf("Digital Output: %d > State: %d\n", Index, State);
	return 0;
}


int ph_kit_SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value)
{
	//printf("Sensor: %d > Value: %d\n", Index, Value);
	ph_kit_laugh(Index,Value);
	return 0;
}


void ph_kit_laugh(int sindex, int svalue)
{
	if(svalue > 0)
		{
			printf("I am Laughing\n");
		}
}


