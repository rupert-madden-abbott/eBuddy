#include "gestures.h"
#include "main.h"



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


    CPhidgetAdvancedServoHandle servo = ph_get_servo_handle();

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
    CPhidgetAdvancedServoHandle servo = ph_get_servo_handle();

    return 0;
}

int gs_raise_arms(void)
{
    CPhidgetAdvancedServoHandle servo = ph_get_servo_handle();
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

