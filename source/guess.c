#include <unistd.h>
#include <time.h>
#include "input.h"
#include "main.h"
#include "utility.h"
#include "debug.h"
#include "gesture_interface.h"
#include "mode.h"
#include "guess.h"

int guess_main(em_State *emotions, qu_queue *notifications, ph_handle *phhandle) {
   int i,j;
   in_input_type input;
   unsigned int iseed = (unsigned int)time(NULL);
   srand (iseed);

   gsi_printLCD("Play the memory game... see if you can repeat my gestures. " 
                "Press my back if I shake my head, or touch whichever hand "
                "I raised", phhandle);

   int sequence[GUESS_NUMBERTOGUESS+1];  // array to store sequence of gestures

   i=0;
   while (i < GUESS_NUMBERTOGUESS) {  // populate sequence of gestures
      j = rand() % GUESS_NUMGESTURES;

      if (j==0) {
         sequence[i] =INPT_LEFT_HAND;
      }

      if (j==1) {
         sequence[i]=INPT_RIGHT_HAND;
      }

      if (j==2) {
         sequence[i]=INPT_FORCE;
      }
      if (i==0 || sequence[i] != sequence[i-1]) {
         i++;
      }
   }
   for (i=0;i<GUESS_NUMBERTOGUESS-GUESS_STARTINGNUMTOGUESS+1;i++) {
      // display sequence of gestures
      for (j=0;j<i+GUESS_STARTINGNUMTOGUESS;j++) {  
         if (sequence[j]==INPT_LEFT_HAND) {
            gsi_raise_left(phhandle);
         }
         if (sequence[j]==INPT_RIGHT_HAND) {
            gsi_raise_right(phhandle);
         }
         if (sequence[j]==INPT_FORCE) {
            gsi_shake_head(phhandle);
         }
      }
      gsi_printLCD("Now let's see if you can remember what I did!", phhandle);
      for (j=0;j<i+GUESS_STARTINGNUMTOGUESS;j++) { 
         do {	// wait for an input
            sleep(1);
            input = in_get_input();
         }
         while(input == INPT_NONE);
         if(input == sequence[j]) {     // user gets the gesture right
            if (j==i+GUESS_STARTINGNUMTOGUESS-1) {
               if (j==GUESS_NUMBERTOGUESS-2) {
                  gsi_printLCD("Perfect! Pay attention... It's the last" 
                               " sequence!", phhandle);
               }
               if (j<GUESS_NUMBERTOGUESS-2) { 
                  gsi_printLCD("Perfect! Pay attention to"
                               " the next sequence...", phhandle);
               }
            }
            else {
               gsi_printLCD("That's right! And the next?", phhandle);
            }   
         }
         else {   // user gets the gesture wrong so print and exit function
            gsi_printLCD("Sorry :( you got that wrong. End of game :(", phhandle);
            return 0;
         }
      }
   }
   // user wins so exit
   gsi_printLCD("You guessed everything! That was fun :)", phhandle);  
   em_update(emotions,EMO_FUN,GUESS_FUNPOINTS);   // add fun points
   return 0;       
}
