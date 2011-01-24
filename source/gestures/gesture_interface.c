#include "gesture_interface.h"
#include "main.h"


int gsi_happy_level1(void)
{
    int status;
    CPhidgetAdvancedServoHandle servo = ph_get_servo_handle();

    CPhidget_getDeviceStatus((CPhidgetHandle)servo, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
    gs_dance_basic(servo);
    return GSI_OK;
}

int gsi_fun_level1(void)
{
    int status;
    CPhidgetAdvancedServoHandle servo = ph_get_servo_handle();
    CPhidget_getDeviceStatus((CPhidgetHandle)servo, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
    gs_raise_arms(servo);
    return GSI_OK;
}

int gsi_notification(void)
{
    gs_sound(3, 4);
    return GSI_OK;
}

int gsi_test(void)
{
    int status;
    CPhidgetAdvancedServoHandle servo = ph_get_servo_handle();
    CPhidget_getDeviceStatus((CPhidgetHandle)servo, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
	gs_move_arms(servo);
	gs_shake_head(servo);

	return GSI_OK;
}

int gsi_eyeflash(void)
{
    int status;
    CPhidgetInterfaceKitHandle ifkit = ph_get_kit_handle();
    CPhidget_getDeviceStatus((CPhidgetHandle)ifkit, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
    gs_eyeflash(ifkit);
    return GSI_OK;
}

int gsi_printLCD(char* str)
{
   CPhidgetTextLCDHandle txt_lcd = lcd_get_lcd_handle();
   lcd_printstring(str,txt_lcd);
   return 0;
}



