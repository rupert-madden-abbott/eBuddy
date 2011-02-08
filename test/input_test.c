#include "input.h"
#include "test.h"

int main(void) {

int input;
for (input =0;input<12;input++)
	{
		if(input==INPT_NONE)
			{
				in_set_input(INPT_NONE);
				input=in_get_input();
		                printf("input from the get function is %d and\
 it should be 0 as the last input\
 was none\n",input);
			}
		else if(input==INPT_BOLTS)
			{
				in_set_input(INPT_BOLTS);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 1 as the last input\
 was bolts\n",input);
			}
		else if(input==INPT_OIL)
			{
				in_set_input(INPT_OIL);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 2 as the last input\
 was oil\n",input);
			}
		else if(input==INPT_BATTERY)
			{
				in_set_input(INPT_BATTERY);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 3 as the last input\
 was battery\n",input);
			}
		else if(input==INPT_FORCE)
			{
				in_set_input(INPT_FORCE);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 4 as the last input\
 was force\n",input);
			}
		else if(input==INPT_DARK)
			{
				in_set_input(INPT_DARK);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 5 as the last input\
 was dark\n",input);
			}
		else if(input==INPT_RIGHT_HAND)
			{
				in_set_input(INPT_RIGHT_HAND);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 6 as the last input\
 was the right hand\n",input);
			}
		else if(input==INPT_LEFT_HAND)
			{
				in_set_input(INPT_LEFT_HAND);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 7 as the last input\
 was the left hand\n",input);
			}
		else if(input==INPT_POWER_ON)
			{
				in_set_input(INPT_POWER_ON);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 8 as the last input\
 was power on\n",input);
			}
		else if(input==INPT_POWER_OFF)
			{
				in_set_input(INPT_POWER_OFF);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 9 as the last input\
 was power off\n",input);
			}
		else if(input==INPT_DEMO)
			{
				in_set_input(INPT_DEMO);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 10 as the last input\
 was demo\n",input);
			}
		else if(input==INPT_DEBUG)
			{
				in_set_input(INPT_DEBUG);
				input=in_get_input();
		                printf("input from the get function is %d and \
it should be 11 as the last input\
 was debug\n",input);
			}
			
		getchar();
	
	}

  return 0;
}
