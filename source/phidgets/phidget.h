#include <phidget21.h>


extern int ph_initialise(void);
extern CPhidgetAdvancedServoHandle ph_get_servo_handle(void);
CPhidgetAdvancedServoHandle ph_servo_initialise(void);
int ph_servo_DetachHandler(CPhidgetHandle phidget_servo, void *p);
int ph_servo_AttachHandler(CPhidgetHandle phidget_servo, void *p);
int ph_servo_ErrorHandler(CPhidgetHandle phidget_servo, void *p, int ErrorCode, const char *Description);

