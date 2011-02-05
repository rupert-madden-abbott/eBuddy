#include "debug.h"

//debug mode main menu
//allows access to all debugging tools
int debug_main(em_State *emotions, qu_queue *notifications) {
  const char *menu[] = {"emotions", "events", "modes"};
  const int menu_size = 3;
  int item, rc;

  //gsi_enter_debug();
  
  //move between different tools until user quits
  do {
  	gsi_printLCD("main menu");

    //display the menu
    item = debug_menu(menu, menu_size);
    rc = 0;
    
    //run the correct command
    switch(item) {
    	
      //emotion editor
      case 0:
        rc = debug_emotions(emotions, notifications);
        break;
      
      //events viewer  
      case 1:
        rc = debug_events(emotions, notifications);
        break;
        
      case 2:
        rc = debug_modes(emotions, notifications);
        
    }
    
    //pass errors to calling function
    if(rc) {
      return rc;
    }
    
  } while(item != DEBUG_EXIT);
  
  gsi_printLCD("exit debug");
  gsi_eyeflash();
  
  return ERR_NONE;
}

//emotion menu allows user to get and set emotion levels
int debug_emotions(em_State *emotions, qu_queue *notifications) {
  const char *action_menu[] = {"get", "set", "update"};
  const int action_menu_size = 3;
  const char *emotion_names[emotions->num_emotions];
  char num_string[DEBUG_BUFF_SIZE];
  int emotion, action;
  float value;
  
  //get the emotion names
  em_get_names(emotions, emotion_names);
  
  //get emotion to perform an action on
  gsi_printLCD("select emotion");
  emotion = debug_menu(emotion_names, emotions->num_emotions);
  
  //check for exit
  if(emotion == DEBUG_EXIT) {
    return ERR_NONE;
  }
  
  //get action to perform
  gsi_printLCD("select action");  
  action = debug_menu(action_menu, action_menu_size);
  
  //check for exit
  if(emotion == DEBUG_EXIT) {
    return ERR_NONE;
  }
  
  //if user selected get print value on the screen
  else if(action == 0) {
  	
  	//read the level of the emotion
  	value = em_get(emotions, emotion);
  	
  	//convert value to string and display it to the user
    sprintf(num_string, "%6lf", value);
    gsi_printLCD(num_string);
  }
  
  //if user selected set change the value
  else if(action == 1) {
  	
  	//get value from user
    gsi_printLCD("select value");
    value = debug_input(0, emotions->emotions[emotion].max, DEBUG_EM_STEP);
    
    em_set(emotions, emotion, value);
    gsi_printLCD("set");
  }
    
    //if user selected update update the value
  else if(action == 2){
    	
    //get value from user
    gsi_printLCD("select value");
    value = debug_input(-emotions->emotions[emotion].max, emotions->emotions[emotion].max, DEBUG_EM_STEP);
      
    em_update(emotions, emotion, value);
    gsi_printLCD("updated");	
  }

  return ERR_NONE;
}


//print event stream to lcd screen
int debug_events(em_State *emotions, qu_queue *notifications) {
  in_input_type input_event;
  em_Event emotion_event;
  nt_message *message;
  char buffer[DEBUG_BUFF_SIZE];
  int running, rc;
  
  gsi_printLCD("push power button to exit");

  //loop until user presses power button
  running = 1;

  while(running) {
  	
  	//look for input events
    input_event = in_get_input();
  
    //check for power button press
    if(input_event == INPT_POWER_OFF) {
      running = 0;
    }
    
    //print other events to the screen
    else if(input_event) {
      sprintf(buffer, "input %02d", input_event);
      gsi_printLCD(buffer);	
    }
  	
  	//look for emotion events
    rc = em_check(emotions, &emotion_event);
  
    //print events on the screen
    if(!rc) {
      sprintf(buffer, "emotion %10s %02d", emotions->emotions[emotion_event.emotion].name, emotion_event.type);
      gsi_printLCD(buffer);
    }
  	
    //get notification events
    message = qu_pop(notifications);
  
    //print details on the screen
    if(message) {
      sprintf(buffer, "message %10s: %10s", message->app, message->user);
      gsi_printLCD(buffer);
      gsi_printLCD(message->text);
    }
  
    sleep(1);
  }
  
  gsi_printLCD("exit");
  return ERR_NONE;
}

//manually change modes
int debug_modes(em_State *emotions, qu_queue *notifications) {
  const char *menu[] = {"react", "sleep", "demo", "debug", "guess"};
  const int menu_size = 5;
  int mode, rc;
  
  gsi_printLCD("select mode");

  //display mode list
  mode = debug_menu(menu, menu_size);
    
  if(mode != DEBUG_EXIT) {
  	
    //switch to chosen mode
    rc = mode_run(mode + 1, emotions, notifications);
        
    //pass errors to calling function
    if(rc) {
      return rc;
    }
  }
  
  return ERR_NONE;
}

//display a menu on the lcd screen allowing the user to choose between
//item's. the function returns the item number or debug_none if the operation
//is canceled
int debug_menu(const char **items, int num_items) {
  in_input_type input;
  int selected, current;

  //start on first item in list
  current = 0;
  selected = 0;
  
  //loop until an item is chosen or the user exits
  while(!selected) {
  	
  	assert(current >= 0);
  	assert(current <= num_items);
  	
    //print the current item on the screen
    gsi_printLCD(items[current]);
    
    //wait for input from the user
    do {
      sleep(1);
      input = in_get_input();
      
    } while(input == INPT_NONE);
  
    //left button selects previous
    if(input == INPT_LEFT_HAND) {
      current = (current == 0) ? num_items - 1 : current - 1;
    }
  
    //right button selects next
    else if(input == INPT_RIGHT_HAND) {
      current = (current == num_items - 1) ? 0 : current + 1;
    }
  
    //force sensor selects item
    else if(input == INPT_FORCE) {
      selected = 1;
    }
    
    //power button or debug key quits debug
    else if(input == INPT_DEBUG || input == INPT_POWER_OFF) {
      current = DEBUG_EXIT;
      selected = 1;
    }
    
  }
  return current;
}

//get a number from the user between min and max with
//steps of the given size
int debug_input(int min, int max, int step) {
  in_input_type input;
  char num_string[DEBUG_BUFF_SIZE];
  int selected, current;

  //make sure step is at least 1
  assert(step > 0);

  //start at average value
  current = (min + max) / 2;
  selected = 0;
  
  //loop until an item is chosen or the user exits
  while(!selected) {
  	
  	//item must be between max and min
  	assert(current >= min);
  	assert(current <= max);
  	
    //print the current number on the screen
    sprintf(num_string, "%8d", current);
    gsi_printLCD(num_string);
    
    //wait for input from the user
    do {
      sleep(1);
      input = in_get_input();
      
    } while(input == INPT_NONE);
  
    //left button increments number
    if(input == INPT_LEFT_HAND) {
      current = (current - step < min) ? min : current - step;
    }
  
    //left button decrements number
    else if(input == INPT_RIGHT_HAND) {
      current = (current +step > max) ? max : current + step;
    }
  
    //force sensor selects number
    else if(input == INPT_FORCE) {
      selected = 1;
    }
    
    //power button or debug key quits debug
    else if(input == INPT_DEBUG || input == INPT_POWER_OFF) {
      current = DEBUG_EXIT;
      selected = 1;
    }
    
  }
  
  return current;
}
