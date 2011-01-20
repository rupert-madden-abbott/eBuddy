#include "em.c"

const em_Emotion emotions[] = {
  {1,	0.0001,	14,	800,	450,	300},
  {1,	0.5,	30,	600,	200,	100},
  {5,	0.0001,	2,	950,	500,	250}
};	

int main() {
  em_State *state;
  em_Event event;
  em_Reaction reaction;
  double val;
  int rc;

  printf("creating state\n");
  state = em_create(emotions, 3);

  if(!state) {
    printf("error creating state\n");
  }
  
  printf("loading file\n");
  rc = em_load(state, "poo.txt");
  
  if(rc) {
    printf("error loading file: %d\n", rc);
  }
  
  rc = 3;
  
  while(rc > 1) {
    printf("emotions: %lf %lf %lf\n", em_get(state, 0), em_get(state, 1), em_get(state, 2));
  	
    rc = em_check(state, &event);
  
    if(rc) {
      printf("Event:: %d %d\n", event.type, event.emotion);
    }
  
    rc = scanf("%d %d %lf", &reaction.action, &reaction.emotion, &reaction.value);
    
    if(rc == 3) {
      printf("update\n");
      em_react(state, &reaction);
    }
  }
  
  
  printf("saving file\n");
  rc = em_save(state, "test.txt");
  
  if(rc) {
    printf("error saving file: %d\n", rc);
  }
  
  printf("destroying state\n");
  em_destroy(state);
  
  
  printf("done\n");
  return 0;
}
