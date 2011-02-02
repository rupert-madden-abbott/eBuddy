#include "gesture_interface.h"


int gsi_gesture_init(ph_handle *handle)
{
    gs_set_pos(*handle->servohandle);
    gs_eyeson(*handle->IFKhandle);
    return 0;
}

int gsi_sound(const char *sound, int repeat)
{
  gs_sound(sound, repeat);
  return 0;
}

int gsi_react(const gsi_Reaction *resp, ph_handle *handle)
{
    int error;
    if(resp->gesture == NULL){
        return GSI_NULL;
    }
    error = resp->gesture(handle);
        
    if(resp->message != NULL){
        error = gsi_printLCD(resp->message, handle);
    }
    if(resp->sound != NULL){
        error = gs_sound(resp->sound, 1);
    }
    return error;
}

int gsi_happy_level1(ph_handle *handle)
{
    return gs_dance_basic(*handle->servohandle);
}

int gsi_fun_level1(ph_handle *handle)
{
    return gs_raise_arms(*handle->servohandle);
    
}


int gsi_test(ph_handle *handle)
{
    int status;
    
	status = gs_move_arms(*handle->servohandle);
	status = gs_shake_head(*handle->servohandle);
	status = gs_turn(*handle->servohandle);
	

	return status;
}

<<<<<<< HEAD
int gsi_wave_right(void)
{
  gs_wave_right(ph_get_servo_handle());
  return 0;
}

int gsi_wave_left(void)
{
  gs_wave_left(ph_get_servo_handle());
  return 0;
}

int gsi_raise_right(void)
{
  gs_raise_right(ph_get_servo_handle());
  return 0;
}
int gsi_raise_left(void)
{
  gs_raise_left(ph_get_servo_handle());
  return 0;
}



int gsi_eyeflash(ph_handle *handle)
{
    
    return gs_eyeflash(*handle->IFKhandle);
}

int gsi_rapid_eyeflash(void)
{
  gs_rapid_eyeflash(ph_get_kit_handle());
  return 0;
}

int gsi_printLCD(const char* str, ph_handle *handle)
{
   return gs_printstring(str, *handle->LCDhandle);
   
}

int gsi_turn(ph_handle *handle)
{
    return gs_turn(*handle->servohandle);
    

}

int gsi_shake_head(ph_handle *handle)
{
    
    return gs_shake_head(*handle->servohandle);
    

}

int gsi_move_arms(ph_handle *handle)
{
    
    return gs_move_arms(*handle->servohandle);
    

}

int gsi_raise_arms(ph_handle *handle)
{
    return gs_raise_arms(*handle->servohandle);

}



