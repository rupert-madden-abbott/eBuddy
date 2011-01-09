#include "gestures.h"


CPhidgetAdvancedServoHandle gestures_get_servo_handle(void)
{
   static int servo_initialised = 0;
   static CPhidgetAdvancedServoHandle servo;
   if(servo_initialised == 0){
       servo_initialised = 1;
       servo = gestures_servo_initialise();


   }
   return servo;

}



CPhidgetAdvancedServoHandle gestures_servo_initialise(void)
{
    double minAccel, maxVel;
    int servo_wait_result;
    const char *servo_attach_error;
    CPhidgetAdvancedServoHandle servo = 0;
    CPhidgetAdvancedServo_create(&servo);
    //CPhidget_set_OnAttach_Handler((CPhidgetHandle)servo, gestures_servo_AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)servo, gestures_servo_DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)servo, gestures_servo_ErrorHandler, NULL);

	CPhidget_open((CPhidgetHandle)servo, -1);

	if((servo_wait_result = CPhidget_waitForAttachment((CPhidgetHandle)servo, 10000)))
	{
		CPhidget_getErrorDescription(servo_wait_result, &servo_attach_error);
		printf("Error eBuddy servo not connected: %s\n", servo_attach_error);
	}


    CPhidgetAdvancedServo_getAccelerationMax(servo, 0, &minAccel);

	CPhidgetAdvancedServo_getVelocityMax(servo, 0, &maxVel);
	printf(" %f %f", minAccel, maxVel);


	gestures_set_pos(servo);




	return servo;



}

int gestures_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p)
{

    printf("eBuddy detatched");

	return 0;
}

//int gestures_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p)
//{

    //printf("eBuddy attached");

	//return 0;
//}

int gestures_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p, int ErrorCode, const char *Description)
{

    printf("eBuddy servo error: %d %s\n", ErrorCode, Description);

	return 0;
}

int gestures_dance_basic(void)
{
    int i;


    CPhidgetAdvancedServoHandle servo = gestures_get_servo_handle();

    CPhidgetAdvancedServo_setEngaged(servo, SERVO_HEAD, 1);
	CPhidgetAdvancedServo_setEngaged(servo, SERVO_TORSO, 1);
	CPhidgetAdvancedServo_setEngaged(servo, SERVO_LEFTARM, 1);
	CPhidgetAdvancedServo_setEngaged(servo, SERVO_RIGHTARM, 1);

    CPhidgetAdvancedServo_setAcceleration(servo, SERVO_HEAD, 160000 );
	CPhidgetAdvancedServo_setVelocityLimit(servo, SERVO_HEAD, 1600);
	CPhidgetAdvancedServo_setAcceleration(servo, SERVO_LEFTARM, 160000 );
	CPhidgetAdvancedServo_setVelocityLimit(servo, SERVO_LEFTARM, 1600);
	CPhidgetAdvancedServo_setAcceleration(servo, SERVO_RIGHTARM, 160000 );
	CPhidgetAdvancedServo_setVelocityLimit(servo, SERVO_RIGHTARM, 1600);

	for(i = 0; i < 2; i++){

	CPhidgetAdvancedServo_setPosition (servo, SERVO_LEFTARM, 100);
	sleep(1);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_LEFTARM, STARTPOS_ARM);
	sleep(1);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_RIGHTARM, 100);
	sleep(1);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_RIGHTARM, STARTPOS_ARM);
	sleep(1);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_HEAD, 100);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_LEFTARM, 100);
	sleep(1);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_HEAD, STARTPOS_HEAD);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_LEFTARM, STARTPOS_ARM);
	sleep(1);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_HEAD, 200);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_RIGHTARM, 100);
	sleep(1);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_HEAD, STARTPOS_HEAD);
	CPhidgetAdvancedServo_setPosition (servo, SERVO_LEFTARM, STARTPOS_ARM);
	sleep(1);
	}


	gestures_set_pos(servo);

	return 0;


}

int gestures_set_pos(CPhidgetAdvancedServoHandle servo)
{


    CPhidgetAdvancedServo_setPosition (servo, SERVO_HEAD, STARTPOS_HEAD);


	CPhidgetAdvancedServo_setEngaged(servo, SERVO_HEAD, 1);
	CPhidgetAdvancedServo_setEngaged(servo, SERVO_TORSO, 1);
	CPhidgetAdvancedServo_setEngaged(servo, SERVO_LEFTARM, 1);
	CPhidgetAdvancedServo_setEngaged(servo, SERVO_RIGHTARM, 1);
	sleep(1);



	CPhidgetAdvancedServo_setEngaged(servo, SERVO_HEAD, 0);
	CPhidgetAdvancedServo_setEngaged(servo, SERVO_TORSO, 0);
	CPhidgetAdvancedServo_setEngaged(servo, SERVO_LEFTARM, 0);
	CPhidgetAdvancedServo_setEngaged(servo, SERVO_RIGHTARM, 0);

    return 0;
}


