#include "gestures.h"
#include "main.h"


CPhidgetAdvancedServoHandle gs_get_servo_handle(void)
{
   static int servo_initialised = 0;
   static CPhidgetAdvancedServoHandle servo;
   if(servo_initialised == 0){
       servo_initialised = 1;
       servo = gs_servo_initialise();


   }
   return servo;

}



CPhidgetAdvancedServoHandle gs_servo_initialise(void)
{
    double minAccel, maxVel;
    int servo_wait_result;
    const char *servo_attach_error;
    CPhidgetAdvancedServoHandle servo = 0;
    CPhidgetAdvancedServo_create(&servo);
    //CPhidget_GS_SET_OnAttach_Handler((CPhidgetHandle)servo, gestures_servo_AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)servo, gs_servo_DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)servo, gs_servo_ErrorHandler, NULL);

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

int gs_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p)
{

    printf("eBuddy detatched");

	return 0;
}

//int gestures_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p)
//{

    //printf("eBuddy attached");

	//return 0;
//}

int gs_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p, int ErrorCode, const char *Description)
{

    printf("eBuddy servo error: %d %s\n", ErrorCode, Description);

	return 0;
}



int gs_set_pos(CPhidgetAdvancedServoHandle servo)
{


    GS_SETPOS (servo, GS_SERVO_HEAD, GS_STARTPOS_HEAD);


	GS_SETENG(servo, GS_SERVO_HEAD, 1);
	GS_SETENG(servo, GS_SERVO_TORSO, 1);
	GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
	GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);
	sleep(1);



	GS_SETENG(servo, GS_SERVO_HEAD, 0);
	GS_SETENG(servo, GS_SERVO_TORSO, 0);
	GS_SETENG(servo, GS_SERVO_LEFTARM, 0);
	GS_SETENG(servo, GS_SERVO_RIGHTARM, 0);

    return 0;
}

int gs_dance_basic(void)
{
    int i;


    CPhidgetAdvancedServoHandle servo = gs_get_servo_handle();

    GS_SETENG(servo, GS_SERVO_HEAD, 1);
	GS_SETENG(servo, GS_SERVO_TORSO, 1);
	GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
	GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);

    GS_SETACC(servo, GS_SERVO_HEAD, 160000 );
	GS_SETVEL(servo, GS_SERVO_HEAD, 1600);
	GS_SETACC(servo, GS_SERVO_LEFTARM, 160000 );
	GS_SETVEL(servo, GS_SERVO_LEFTARM, 1600);
	GS_SETACC(servo, GS_SERVO_RIGHTARM, 160000 );
	GS_SETVEL(servo, GS_SERVO_RIGHTARM, 1600);

	for(i = 0; i < 2; i++){

	GS_SETPOS (servo, GS_SERVO_LEFTARM, 100);
	sleep(1);
	GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_ARM);
	sleep(1);
	GS_SETPOS (servo, GS_SERVO_RIGHTARM, 100);
	sleep(1);
	GS_SETPOS (servo, GS_SERVO_RIGHTARM, GS_STARTPOS_ARM);
	sleep(1);
	GS_SETPOS (servo, GS_SERVO_HEAD, 100);
	GS_SETPOS (servo, GS_SERVO_LEFTARM, 100);
	sleep(1);
	GS_SETPOS (servo, GS_SERVO_HEAD, GS_STARTPOS_HEAD);
	GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_ARM);
	sleep(1);
	GS_SETPOS (servo, GS_SERVO_HEAD, 200);
	GS_SETPOS (servo, GS_SERVO_RIGHTARM, 100);
	sleep(1);
	GS_SETPOS (servo, GS_SERVO_HEAD, GS_STARTPOS_HEAD);
	GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_ARM);
	sleep(1);
	}


	gs_set_pos(servo);

	return 0;


}

int gs_dance_advanced(void)
{
    CPhidgetAdvancedServoHandle servo = gs_get_servo_handle();

    return 0;
}

int gs_raise_arms(void)
{
    CPhidgetAdvancedServoHandle servo = gs_get_servo_handle();
    GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
	GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);
    GS_SETPOS (servo, GS_SERVO_LEFTARM, 200);
    GS_SETPOS (servo, GS_SERVO_RIGHTARM, 200);
    sleep(1);
    gs_set_pos(servo);

    return 0;
}

int gs_sound(int sound, int itineration)
{
    int i = 0;
    while(i < itineration){
        i++;
        switch(sound){
            case 0:
                system("mpg123 -q beep1.mp3");
                break;
            case 1:
                system("mpg123 -q robotsound1.mp3");
                break;
            case 2:
                system("mpg123 -q tinkle.mp3");
                break;
        }



    }
    return 0;
}

