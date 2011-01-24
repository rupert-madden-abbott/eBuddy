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

if(tagv==oil)
	{
		if(in_last_input==nothing)
		{
			in_last_input=oil;//eating
		}
	}
if(tagv==battery)
	{
		if(in_last_input==nothing)
		{
			in_last_input=battery;//drinking
		}
	}
if(tagv==nuts_and_bolts)
	{
		if(in_last_input==nothing)
		{
			in_last_input=nuts_and_bolts;//drinking
		}
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
	if(svalue > 0)
		{
			printf("I am Laughing\n");
		}
}

