#include <stdio.h>
#include <unistd.h>

#include "emotion.h"

const em_Emotion EMOTIONS[] = {
  
//name        life           factor   alert         max    full  low  critical
  {"hunger",  UT_HOUR * 24,  0.2,     UT_MIN * 5,   100,   95,   30,  10},
  {"energy",  UT_HOUR * 18,  0.5,     UT_MIN * 13,  100,   95,   20,  10},
  {"clean",   UT_HOUR * 24,  0.3,     UT_MIN * 7,   150,   100,  50,  10},
  {"social",  UT_HOUR * 2,   0.9,     UT_MIN * 9,   100,   98,   40,  20},
  {"fun",     UT_HOUR * 2,   0.7,     UT_MIN * 11,  100,   98,   20,  5}
};

const int NUM_EMOTIONS = 5;


void file_test(void);
void decay_test(void);


int main(void) {
  int test, rc;

  do {
    printf("select a test:\n");
    printf("1> file test\n");
    printf("2> decay test\n");
    printf("3> exit\n");
    
    rc = scanf("%d", &test);
    
  } while(rc != 1);
  
  if(test == 1){
  	file_test();

  }

  else if(test == 2) {
    decay_test();
  }
  
  return 0;
}

void file_test(void) {
  em_State *state;
  em_State *control;
  double val, control_val;
  int num_cycles, errors;
  int i, rc;
  
  do{
  	printf("How many cycles\n");
    rc = scanf("%d", &num_cycles);
    
  } while(rc != 1);

  //create state and control state
  state = em_init(EMOTIONS, NUM_EMOTIONS);
  control = em_init(EMOTIONS, NUM_EMOTIONS);
  
  errors = 0;
  
  //load and save many times
  for(i = 0; i < num_cycles; i++) {
    em_save(state, "em_test_state");
    em_load(state, "em_test_state");
  }
  
  //test each level
  for(i = 0; i < NUM_EMOTIONS; i++) {
    val = em_get(state, i);
    control_val = em_get(state, i);
    
    if(val != control_val) {
      printf("Error, got %lf but expected %lf\n", val, control_val);
      errors++;
    }
  }
  
  printf("Test completed with %d errors\n", errors);
}

void decay_test(void) {
  em_State *state;
  em_Event event;
  double val;
  int i, rc;

  //create state and control state
  state = em_init(EMOTIONS, NUM_EMOTIONS);
  
  //print all emotions once each second
  while(1){
    for(i = 0; i < NUM_EMOTIONS; i++) {
      val = em_get(state, i);
      printf("%s: %lf\t", EMOTIONS[i].name, val);	
    }
    printf("\n");
    
    //print events
    rc = em_check(state, &event);
  
    //print events on the screen
    if(!rc) {
      printf("event: %10s %02d\n",
              state->emotions[event.emotion].name,
              event.type);
    }
    
    sleep(1);
  }
}
