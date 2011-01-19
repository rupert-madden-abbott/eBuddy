#include "phidget.h"
#include "main.h"
#include "gestures.h"


int ph_initialise(void)
{
    ph_get_servo_handle();

    return 0;
}

int ph_destruct(void)
{
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

