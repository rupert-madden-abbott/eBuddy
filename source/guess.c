#include <unistd.h>
#include <time.h>
#include "input.h"
#include "main.h"
#include "utility.h"
#include "debug.h"
#include "gesture_interface.h"
#include "mode.h"
#include "guess.h"

int guess_main(em_State *emotions, qu_queue *notifications) {
   int i,j;
   in_input_type input;
   unsigned int iseed = (unsigned int)time(NULL);
   srand (iseed);

   gsi_printLCD("Play the memory game... see if you can repeat my gestures " 
                "Press my back if I shake my head, or touch whichever hand "
                "I raised");

   int sequence[GUESS_NUMBERTOGUESS+1];  // array to store sequence of gestures

   for (i=0;i<GUESS_NUMBERTOGUESS;i++) {  // populate sequence of gestures
      j=rand()%GUESS_NUMGESTURES;
      
      if (j%GUESS_NUMGESTURES==0) {
         sequence[i]=INPT_LEFT_HAND;
      }
      if (j%GUESS_NUMGESTURES==1) {
         sequence[i]=INPT_RIGHT_HAND;
      }
      if (j%GUESS_NUMGESTURES==2) {
         sequence[i]=INPT_FORCE;
      }
   }
   while (1) {
      for (i=0;i<GUESS_NUMBERTOGUESS;i++) {
         for (j=0;j<i+GUESS_STARTINGNUMTOGUESS;j++) {  // display sequence of gestures
            if (sequence[j]==INPT_LEFT_HAND) {
               gsi_raise_left();
            }
            if (sequence[j]==INPT_RIGHT_HAND) {
               gsi_raise_right();
            }
            if (sequence[j]==INPT_FORCE) {
               gsi_shake_head();
            }
         }
         gsi_printLCD("Now let's see if you can remember what I did!");
         for (j=0;j<i+GUESS_STARTINGNUMTOGUESS;j++) { 
            do {	// wait for an input
               sleep(1);
               input = in_get_input();
            }
            while(input == INPT_NONE);
            if(input == sequence[j]) {     // user gets the gesture right
               if (j==i+GUESS_STARTINGNUMTOGUESS-1) {
                  gsi_printLCD("Perfect! Pay attention to the next sequence..."); 
               }
               else {
                  gsi_printLCD("That's right! And the next?");
               }   
            }
            else {   // user gets the gesture wrong
               gsi_printLCD("Sorry :( you got that wrong. End of game :(");
               return 0;
            }
         }
      }
      gsi_printLCD("You guessed everything! That was fun :)");  // user wins so exit
      em_update(emotions,EMO_FUN,GUESS_FUNPOINTS);
      return 0;       
   }
   return 0;
}
