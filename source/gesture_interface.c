#include "gesture_interface.h"

int gsi_react(const gsi_Reaction *resp, Phhandle handle)
{
    int error = GSI_OK;
    if(resp->gesture == NULL){
        return GSI_NULL;
    }
    if(resp->gesture(handle) == GSI_PHER){
        error = GSI_PHER;
    }
    if(resp->message != NULL){
        if(gsi_printLCD(resp->message, handle->LCDhandle) == GSI_PHER){
            error = GSI_PHER;
        }
    }
    return error;
}

int gsi_happy_level1(Phandle handle)
{
    int status;
    CPhidget_getDeviceStatus((CPhidgetHandle)handle->servohandle, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
    gs_dance_basic(handle->servohandle);
    return GSI_OK;
}

int gsi_fun_level1(Phandle handle)
{
    int status;
    
    CPhidget_getDeviceStatus((CPhidgetHandle)handle->servohandle, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
    gs_raise_arms(handle->servohandle);
    return GSI_OK;
}

int gsi_notification(void)
{
    gs_sound(7, 1);
    return GSI_OK;
}

int gsi_test(Phandle handle)
{
    int status;
    
    CPhidget_getDeviceStatus((CPhidgetHandle)handle->servohandle, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
	gs_move_arms(handle->servohandle);
	gs_shake_head(handle->servohandle);
	gs_turn(handle->servohandle);
	

	return GSI_OK;
}

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


int gsi_eyeflash(Phandle handle)
{
    int status;
    
    CPhidget_getDeviceStatus((CPhidgetHandle)handle->IFKhandle, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
    gs_eyeflash(handle->IFKhandle);
    return GSI_OK;
}

int gsi_rapid_eyeflash(void)
{
  gs_rapid_eyeflash(ph_get_kit_handle());
  return 0;
}

int gsi_printLCD(const char* str, Phandle handle)
{
    int status;
    CPhidget_getDeviceStatus((CPhidgetHandle)handle->LCDhandle, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
   gs_printstring(str,txt_lcd, handle->LCDhandle);
   return GSI_OK;
}

int gsi_turn(Phandle handle)
{
    int status;
    CPhidget_getDeviceStatus((CPhidgetHandle)handle->servohandle, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
    gs_turn(handle->servohandle);
    return GSI_OK;

}

int gsi_shake_head(Phandle handle)
{
    int status;
    CPhidget_getDeviceStatus((CPhidgetHandle)handle->servohandle, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
    gs_shake_head(handle->servohandle);
    return GSI_OK;

}

int gsi_move_arms(Phandle handle)
{
    int status;
    
    CPhidget_getDeviceStatus((CPhidgetHandle)handle->servohandle, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
    gs_move_arms(handle->servohandle);
    return GSI_OK;

}

int gsi_raise_arms(Phandle handle)
{
    int status;
    
    CPhidget_getDeviceStatus((CPhidgetHandle)handle->servohandle, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
    gs_raise_arms(handle->servohandle);
    return GSI_OK;

}



