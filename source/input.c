#include "input.h"



int in_input_init(ph_handle *handle)
{
    in_RFID_init(*handle->RFIDhandle);
    in_kit_input_init(*handle->IFKhandle);
    return 0;
}

int in_RFID_init(CPhidgetRFIDHandle RFID)
{
    CPhidgetRFID_set_OnTag_Handler(RFID, in_RFID_TagHandler, NULL);
    CPhidgetRFID_set_OnTagLost_Handler(RFID, in_RFID_TagLostHandler, NULL);
    CPhidget_open((CPhidgetHandle)RFID, -1);
    CPhidgetRFID_setAntennaOn(RFID, 1);
    return 0;

}

int in_kit_input_init(CPhidgetInterfaceKitHandle ifKit)
{
    CPhidgetInterfaceKit_set_OnInputChange_Handler (ifKit, in_kit_InputChangeHandler, NULL);
	CPhidgetInterfaceKit_set_OnSensorChange_Handler (ifKit, in_kit_SensorChangeHandler, NULL);
    return 0;
}

int in_last_input;
/* RFID input*/
int in_RFID_TagHandler(CPhidgetRFIDHandle RFID, void *usrptr, unsigned char *TagVal)
{
	//turn on the Onboard LED
	CPhidgetRFID_setLEDOn(RFID, 1);
	
	printf("Tag Read: %02x%02x%02x%02x%02x\n", TagVal[0], TagVal[1], TagVal[2], TagVal[3], TagVal[4]);
	in_RFID_savetag(TagVal[0]+TagVal[1]+TagVal[2]+TagVal[3]+TagVal[4]);
	return 0;
}


int in_RFID_TagLostHandler(CPhidgetRFIDHandle RFID, void *usrptr, unsigned char *TagVal)
{
	//turn off the Onboard LED
	CPhidgetRFID_setLEDOn(RFID, 0);

	printf("Tag Lost: %02x%02x%02x%02x%02x\n", TagVal[0], TagVal[1], TagVal[2], TagVal[3], TagVal[4]);
	return 0;
}

void in_RFID_savetag(int tagv)
{

if(tagv==IN_DISC)
	{
		
			in_set_input(INPT_OIL);//oil
		
	}
if(tagv==IN_CARDONE)
	{
		
			in_set_input(INPT_BATTERY);//battery
		
	}
if(tagv==IN_CARDTWO)
	{
		
			in_set_input(INPT_DEMO);//DEMO
		
	}
if(tagv==IN_KEYFOBONE)
	{
	
			in_set_input(INPT_BOLTS);//nuts and bolts
		
	}
if(tagv==IN_KEYFOBTWO)
	{
	
			in_set_input(INPT_DEBUG);//DEBUG
		
	}

}

/*Interface Kit input*/
int in_kit_InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
	//printf("Digital Input: %d > State: %d\n", Index, State);
	return 0;
}

int in_kit_SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value)
{
	//printf("Sensor: %d > Value: %d\n", Index, Value);
	in_kit_laugh(Index,Value);
	return 0;
}

void in_kit_laugh(int sindex, int svalue)
{
	if(svalue > IN_PRESS && sindex==IN_FORCE)
		{
			static time_t last=0;
			time_t current;
			double dif;
			time(&current);
			if(last==0)
				{
					last=current;
					in_set_input(INPT_FORCE);//FORCE
				}
			else
				{
					dif= difftime(current,last);
					last=current;
					if(dif>1)
						{
							in_set_input(INPT_FORCE);//FORCE
						}
				}

			
		}
	else if (svalue==0 && sindex==IN_TOUCHONE)
		{
			in_set_input(INPT_RIGHT_HAND);//TOUCH ONE(RIGHT HAND)
		}
	else if (svalue==0 && sindex==IN_TOUCHTWO)
		{
			in_set_input(INPT_LEFT_HAND);//TOUCH TWO(LEFT HAND)
		}
}

/*in_input_type functions*/
int in_get_input(void)

{
int temp;
temp=in_last_input;
in_last_input=INPT_NONE;
return temp;
}
void in_set_input(int input)
{
if(in_last_input==INPT_NONE)
	{
		in_last_input=input;
	}

}
