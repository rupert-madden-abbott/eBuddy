/**
 *@file gesture_interface.c
 *@author Andrew Barber
 *
 */



#include "gesture_interface.h"

//initialise gesture phidgets
int gsi_gesture_init(ph_handle *handle) {
  gs_set_pos(handle->servohandle);
  gs_eyeson(handle->IFKhandle);
  CPhidgetTextLCD_setContrast (handle->LCDhandle, 100);
  return 0;
}

//shut down gesture phidgets
int gsi_gesture_close(ph_handle *handle) {
  gsi_sound("servo1", 1);
  gs_set_pos(handle->servohandle);
  gs_eyesoff(handle->IFKhandle);
  return 0;
}

int gsi_sound(const char *sound, int repeat) {
  return gs_sound(sound, repeat);
  
}

int gsi_react(const gsi_Reaction *resp, ph_handle *handle) {
  int error;
  //if no function pointer specified return error
  if(resp->gesture == NULL){
    return GSI_NULL;
  }
  error = resp->gesture(handle);
  
  //if message print to LCD screen      
  if((resp->message != NULL) && (resp->message[0] != '\0')){
    error = gsi_printLCD(resp->message, handle);
  }
  //if sound play sound
  if((resp->sound != NULL) && (resp->sound[0] != '\0')){
    error = gs_sound(resp->sound, 1);
  }
  return error;
}

int gsi_happy_level1(ph_handle *handle) {
  return gs_dance_basic(handle->servohandle);
}

int gsi_fun_level1(ph_handle *handle) {
  return gs_raise_arms(handle->servohandle);
    
}


int gsi_test(ph_handle *handle) {
  int status;
  status = gs_move_arms(handle->servohandle);
  status = gs_shake_head(handle->servohandle);
  status = gs_turn(handle->servohandle);
  return status;
}

int gsi_wave_right(ph_handle *handle) {
  gs_wave_right(handle->servohandle);
  return 0;
}

int gsi_wave_left(ph_handle *handle) {
  gs_wave_left(handle->servohandle);
  return 0;
}

int gsi_raise_right(ph_handle *handle)
{
  gs_raise_right(handle->servohandle);
  return 0;
}
int gsi_raise_left(ph_handle *handle) {
  gs_raise_left(handle->servohandle);
  return 0;
}



int gsi_eyeflash(ph_handle *handle) {
    
  return gs_eyeflash(handle->IFKhandle);
}

int gsi_rapid_eyeflash(ph_handle *handle) {
  gs_rapid_eyeflash(handle->IFKhandle);
  return 0;
}

int gsi_printLCD(const char* str, ph_handle *handle) {
  return gs_printstring(str, handle->LCDhandle);
   
}

int gsi_turn(ph_handle *handle) {
  return gs_turn(handle->servohandle);
}

int gsi_shake_head(ph_handle *handle) {
    
  return gs_shake_head(handle->servohandle);
    
}

int gsi_move_arms(ph_handle *handle) {
    
  return gs_move_arms(handle->servohandle);
}

int gsi_raise_arms(ph_handle *handle) {
  gsi_sound("servo2", 1);
  return gs_raise_arms(handle->servohandle);
}



