#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <phidget21.h>
#include <jansson.h>
#include "utility.h"
#include "phidget.h"
#include "config.h"
#include "gesture.h"
#include "gesture_interface.h"

int gsi_react(gsi_Reaction *resp)
{
    int error = GSI_OK;
    if(resp->gesture == NULL){
        return GSI_NULL;
    }
    if(resp->gesture() == GSI_PHER){
        error = GSI_PHER;
    }
    if(resp->message != NULL){
        if(gsi_printLCD(resp->message) == GSI_PHER){
            error = GSI_PHER;
        }
    }
    return GSI_OK;
}

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
	gs_turn(servo);
	

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

int gsi_printLCD(const char* str)
{
    int status;
    CPhidgetTextLCDHandle txt_lcd = ph_get_lcd_handle();
    CPhidget_getDeviceStatus((CPhidgetHandle)txt_lcd, &status);
    if(status == PHIDGET_NOTATTACHED){
        return GSI_PHER;
    }
   gs_printstring(str,txt_lcd);
   return GSI_OK;
}



