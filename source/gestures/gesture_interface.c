#include "gesture_interface.h"
#include "main.h"
#include "gestures.h"

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
    gs_sound(0, 1);
    return GSI_OK;
}


