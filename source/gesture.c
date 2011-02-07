#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <phidget21.h>

#include "config.h"
#include "utility.h"
#include "phidget.h"
#include "gesture.h"

/*servo gestures*/

//set default servo position
int gs_set_pos(CPhidgetAdvancedServoHandle servo) {

  int error;
  GS_SETENG(servo, GS_SERVO_HEAD, 1);
  GS_SETENG(servo, GS_SERVO_TORSO, 1);
  GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
  GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);
  GS_SETPOS (servo, GS_SERVO_HEAD, GS_STARTPOS_HEAD);
  GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_LEFTARM);
  GS_SETPOS (servo, GS_SERVO_RIGHTARM, GS_STARTPOS_RIGHTARM);
  GS_SETPOS (servo, GS_SERVO_TORSO, GS_STARTPOS_TORSO);
  usleep(UT_MICRO * 1000);
  GS_SETENG(servo, GS_SERVO_HEAD, 0);
  GS_SETENG(servo, GS_SERVO_TORSO, 0);
  GS_SETENG(servo, GS_SERVO_LEFTARM, 0);
  GS_SETENG(servo, GS_SERVO_RIGHTARM, 0);

  return 0;
}

int gs_dance_basic(CPhidgetAdvancedServoHandle servo) {
  int i;
  int error;

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

  for(i = 0; i < 2; i++) {

    GS_SETPOS (servo, GS_SERVO_LEFTARM, 100);
    usleep(UT_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_LEFTARM);
    usleep(UT_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_RIGHTARM, 100);
    usleep(UT_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_RIGHTARM, GS_STARTPOS_RIGHTARM);
    usleep(UT_MICRO * 500);;
    GS_SETPOS (servo, GS_SERVO_HEAD, 100);
    GS_SETPOS (servo, GS_SERVO_LEFTARM, 100);
    usleep(UT_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_HEAD, GS_STARTPOS_HEAD);
    GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_LEFTARM);
    usleep(UT_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_HEAD, 200);
    GS_SETPOS (servo, GS_SERVO_RIGHTARM, 100);
    usleep(UT_MICRO * 500);
    GS_SETPOS (servo, GS_SERVO_HEAD, GS_STARTPOS_HEAD);
    GS_SETPOS (servo, GS_SERVO_LEFTARM, GS_STARTPOS_RIGHTARM);
    usleep(UT_MICRO * 500);
  }


  gs_set_pos(servo);

  return 0;

}

int gs_dance_advanced(CPhidgetAdvancedServoHandle servo)
{


   return 0;
}

int gs_raise_arms(CPhidgetAdvancedServoHandle servo) {
  int error;
  GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
  GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);
  GS_SETPOS (servo, GS_SERVO_LEFTARM, 200);
  GS_SETPOS (servo, GS_SERVO_RIGHTARM, 20);
  usleep(UT_MICRO * 1000);
  gs_set_pos(servo);

  return 0;
}

int gs_sound(const char *sound, int repeat) {
  int i = 0;
  char filepath[100];
  char command[110];
  cf_json *root;

  root = cf_read("conf/sound.json");
  if(!root) return 1;

  strncpy(filepath, cf_get_string(root, sound), 100);
  cf_free(root);

  command[0] ='\0';
  strcpy(command, "mpg123 -q & ");
  strcat(command, filepath);
  while(i < repeat){
    system(command);
    i++;

  }
  return 0;
}

int gs_shake_head(CPhidgetAdvancedServoHandle servo) {
  int error;
  GS_SETENG(servo, GS_SERVO_HEAD, 1);
  GS_SETPOS (servo, GS_SERVO_HEAD, 200);
  usleep(UT_MICRO * 500);
  GS_SETPOS (servo, GS_SERVO_HEAD, 20);
  usleep(UT_MICRO * 500);
  GS_SETPOS (servo, GS_SERVO_HEAD, 200);
  usleep(UT_MICRO * 500);
  GS_SETPOS (servo, GS_SERVO_HEAD, 20);
  usleep(UT_MICRO * 500);
  gs_set_pos(servo);

  return 0;
}

int gs_move_arms(CPhidgetAdvancedServoHandle servo) {
  int error;
  GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
  GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);
  GS_SETPOS (servo, GS_SERVO_LEFTARM, 200);
  GS_SETPOS (servo, GS_SERVO_RIGHTARM, 200);
  usleep(UT_MICRO * 500);
  GS_SETPOS (servo, GS_SERVO_LEFTARM, 20);
  GS_SETPOS (servo, GS_SERVO_RIGHTARM, 20);
  usleep(UT_MICRO * 500);
  GS_SETPOS (servo, GS_SERVO_LEFTARM, 200);
  GS_SETPOS (servo, GS_SERVO_RIGHTARM, 200);
  usleep(UT_MICRO * 500);
  gs_set_pos(servo);

  return 0;
}

int gs_turn(CPhidgetAdvancedServoHandle servo) {
  int error;
  GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
  GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);
  GS_SETENG(servo, GS_SERVO_HEAD, 1);
  GS_SETENG(servo, GS_SERVO_TORSO, 1);

  GS_SETPOS(servo, GS_SERVO_LEFTARM, 100);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 100);
  usleep(UT_MICRO * 500);
  GS_SETPOS(servo, GS_SERVO_TORSO, 50);
  GS_SETPOS(servo, GS_SERVO_HEAD, 160);
  usleep(UT_MICRO * 2000);
  GS_SETPOS(servo, GS_SERVO_TORSO, GS_STARTPOS_TORSO);
  GS_SETPOS(servo, GS_SERVO_HEAD, GS_STARTPOS_HEAD);
  usleep(UT_MICRO * 1000);
  GS_SETPOS(servo, GS_SERVO_TORSO, 160);
  GS_SETPOS(servo, GS_SERVO_HEAD, 60);
  usleep(UT_MICRO * 2000);
  gs_set_pos(servo);

  return 0;
}

/*LED functions*/
int gs_eyesoff(CPhidgetInterfaceKitHandle ifkit) {
  
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  return 0;
}

int gs_eyeson(CPhidgetInterfaceKitHandle ifkit) {
   
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PTRUE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PTRUE);
  return 0;
}

int gs_eyeflash(CPhidgetInterfaceKitHandle ifkit) {
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  usleep(UT_MICRO * 250);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PTRUE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PTRUE);
  usleep(UT_MICRO * 250);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  usleep(UT_MICRO * 250);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PTRUE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PTRUE);
  usleep(UT_MICRO * 250);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  usleep(UT_MICRO * 250);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PTRUE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PTRUE);
  return 0;
}

int gs_rapid_eyeflash(CPhidgetInterfaceKitHandle ifkit) {
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PTRUE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PTRUE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PTRUE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PTRUE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PTRUE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PTRUE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PTRUE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PTRUE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PTRUE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PTRUE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  usleep(UT_MICRO * 80);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_RIGHTEYE, PFALSE);
  CPhidgetInterfaceKit_setOutputState (ifkit, GS_LEFTEYE, PFALSE);
  return 0;
}

int gs_wave_right(CPhidgetAdvancedServoHandle servo) {
  int error;
  GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 80);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 100);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 80);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 100);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 80);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 100);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 80);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 100);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 80);
  usleep(UT_MICRO * 300);
  gs_set_pos(servo);
  return 0;
  
}

int gs_raise_left(CPhidgetAdvancedServoHandle servo) {
  int error;
  GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
  GS_SETPOS(servo, GS_SERVO_LEFTARM, 120);
  usleep(UT_MICRO * 1500);
  gs_set_pos(servo);
  return 0;
}

int gs_raise_right(CPhidgetAdvancedServoHandle servo) {
  int error;
  GS_SETENG(servo, GS_SERVO_RIGHTARM, 1);
  GS_SETPOS(servo, GS_SERVO_RIGHTARM, 80);
  usleep(UT_MICRO * 1500);
  gs_set_pos(servo);
  return 0;
}

int gs_wave_left(CPhidgetAdvancedServoHandle servo) {
  int error;
  GS_SETENG(servo, GS_SERVO_LEFTARM, 1);
  GS_SETPOS(servo, GS_SERVO_LEFTARM, 120);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_LEFTARM, 100);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_LEFTARM, 120);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_LEFTARM, 100);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_LEFTARM, 120);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_LEFTARM, 100);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_LEFTARM, 120);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_LEFTARM, 100);
  usleep(UT_MICRO * 300);
  GS_SETPOS(servo, GS_SERVO_LEFTARM, 120);
  usleep(UT_MICRO * 300);
  gs_set_pos(servo);
  return 0;
  
}

int gs_printstring(const char* string,CPhidgetTextLCDHandle txt_lcd) {
  int flag = 0;
  int length = strlen(string);
  int j;
  int row = 0, startpoint = 0;
  char tmp[10000];
  char str1[10000];
  char blank[1] = "\0";
  
  // clear LCD row 0
  CPhidgetTextLCD_setDisplayString (txt_lcd,0,blank); 

  // clear LCS row 1
  CPhidgetTextLCD_setDisplayString (txt_lcd,1,blank);  

  // turn on backlight
  CPhidgetTextLCD_setBacklight(txt_lcd,1); 

  // copy string into tmp
  for (j = 0; j < length; j++) {
    tmp[j]=string[j];
  }

  tmp[length] = WHITESPACE;
  tmp[length+1]='\0';

  // loop splits the string into substrings no more than LCDWIDTH characters
  // 2 lines are printed to the screen at a time
  while (startpoint < length) {
    for (j = startpoint + LCDWIDTH; j > startpoint; j--) {
      // find the start of the last word on the row
      if (tmp[j-1] == WHITESPACE && flag == 0) {
        strncpy(str1, tmp + startpoint, j - startpoint - 1);
        str1[j - startpoint - 1] = '\0';

        // print str1 to the lcd on row number 'row'
        CPhidgetTextLCD_setDisplayString (txt_lcd,row,str1);

        if (row == 0) {
          row = 1;
        }

        else {
          row = 0;
        }

        strcpy(str1,"");
        flag = 1;
        startpoint = j;
      }
    }

    // if flag==0 at this point then there is a word that is greater than
    // LCDWIDTH characters
    if (flag == 0) {
      strncpy(str1,tmp+startpoint,LCDWIDTH);
      str1[LCDWIDTH] = '\0';
      CPhidgetTextLCD_setDisplayString (txt_lcd,row,str1);

      if (row == 0) {
        row = 1;
      }

      else {
        row = 0;
      }

      strcpy(str1,"");
      flag = 1;
      startpoint = startpoint + LCDWIDTH;
    }

    flag = 0;

    // if last printed row was row 1 then row == 0 so sleep
    if (row == 0) {
      usleep(SCROLLSPEED);
    }
  }

  if (row == 1) {
    CPhidgetTextLCD_setDisplayString (txt_lcd,1,blank);
    usleep(SCROLLSPEED);
  }

  // clear the LCD and turn off the backlight
  CPhidgetTextLCD_setBacklight(txt_lcd,0);
  CPhidgetTextLCD_setDisplayString (txt_lcd,0,blank);
  CPhidgetTextLCD_setDisplayString (txt_lcd,1,blank);

  return 0;
}


