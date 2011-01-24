#include "gestures.h"

int gs_init(PhidgetHandle *phidgets, const char *config) {
  return 0;
}




int gs_set_pos(CPhidgetAdvancedServoHandle servo)
{


    GS_SETENG(servo, GS_SERVO_HEAD, 1);
	GS_SETENG(servo, GS_SERVO_TORSO, 1);
	GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
	GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);

	GS_SETPOS (servo, GS_SERVO_HEAD, GS_STARTPOS_HEAD);
    GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_LEFTARM);
    GS_SETPOS (servo, GS_SERVO_RIGHTARM, GS_STARTPOS_RIGHTARM);
    GS_SETPOS (servo, GS_SERVO_TORSO, GS_STARTPOS_TORSO);



	usleep(GS_MICRO * 1000);



	GS_SETENG(servo, GS_SERVO_HEAD, 0);
	GS_SETENG(servo, GS_SERVO_TORSO, 0);
	GS_SETENG(servo, GS_SERVO_LEFTARM, 0);
	GS_SETENG(servo, GS_SERVO_RIGHTARM, 0);

    return 0;
}

int gs_dance_basic(CPhidgetAdvancedServoHandle servo)
{
    int i;


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
	usleep(GS_MICRO * 500);
	GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_LEFTARM);
	usleep(GS_MICRO * 500);
	GS_SETPOS (servo, GS_SERVO_RIGHTARM, 100);
	usleep(GS_MICRO * 500);
	GS_SETPOS (servo, GS_SERVO_RIGHTARM, GS_STARTPOS_RIGHTARM);
	usleep(GS_MICRO * 500);;
	GS_SETPOS (servo, GS_SERVO_HEAD, 100);
	GS_SETPOS (servo, GS_SERVO_LEFTARM, 100);
	usleep(GS_MICRO * 500);
	GS_SETPOS (servo, GS_SERVO_HEAD, GS_STARTPOS_HEAD);
	GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_LEFTARM);
	usleep(GS_MICRO * 500);
	GS_SETPOS (servo, GS_SERVO_HEAD, 200);
	GS_SETPOS (servo, GS_SERVO_RIGHTARM, 100);
	usleep(GS_MICRO * 500);
	GS_SETPOS (servo, GS_SERVO_HEAD, GS_STARTPOS_HEAD);
	GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_RIGHTARM);
	usleep(GS_MICRO * 500);
	}


	gs_set_pos(servo);

	return 0;


}

int gs_dance_advanced(CPhidgetAdvancedServoHandle servo)
{


    return 0;
}

int gs_raise_arms(CPhidgetAdvancedServoHandle servo)
{
    GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
	GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);
    GS_SETPOS (servo, GS_SERVO_LEFTARM, 200);
    GS_SETPOS (servo, GS_SERVO_RIGHTARM, 20);
    usleep(GS_MICRO * 1000);
    gs_set_pos(servo);

    return 0;
}

int gs_sound(int sound, int itineration)
{
    int i = 0;
    const char *filepath = NULL;
    char command[110];
    char num[10];
    cf_json *root;
    sprintf(num, "%d", sound);
   
    root = cf_read("conf/sound.json");
    if(!root) return 1;
    
    filepath = cf_get_string(root, num);
    cf_free(root);
    if(!filepath) return 1;
    
    command[0] ='\0';
    strcpy(command, "mpg123 -q ");
    strcat(command, filepath);
    while(i < itineration){
        system(command);
        i++;

    }
    return 0;
}

int gs_shake_head(CPhidgetAdvancedServoHandle servo)
{
    GS_SETENG(servo, GS_SERVO_HEAD, 1);
    GS_SETPOS (servo, GS_SERVO_HEAD, 200);
    usleep(GS_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_HEAD, 20);
    usleep(GS_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_HEAD, 200);
    usleep(GS_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_HEAD, 20);
    usleep(GS_MICRO * 500);
    gs_set_pos(servo);

    return 0;
}

int gs_move_arms(CPhidgetAdvancedServoHandle servo)
{
    GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
	GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);
    GS_SETPOS (servo, GS_SERVO_LEFTARM, 200);
    GS_SETPOS (servo, GS_SERVO_RIGHTARM, 200);
    usleep(GS_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_LEFTARM, 20);
    GS_SETPOS (servo, GS_SERVO_RIGHTARM, 20);
    usleep(GS_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_LEFTARM, 200);
    GS_SETPOS (servo, GS_SERVO_RIGHTARM, 200);
    usleep(GS_MICRO * 500);
    gs_set_pos(servo);

    return 1;
}

