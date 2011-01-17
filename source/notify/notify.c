#include "notify.h"
#include "config.h"

int noti_initialize(void) {
  int ask_details, have_details;
  char username[100], password[100];
  
  ask_details = atoi(conf_read(NOTI_CONF_FILE, "twitter", "ask_details"));
  have_details = atoi(conf_read(NOTI_CONF_FILE, "twitter", "have_details"));
  
  if(!have_details && ask_details) {
    printf("Enter your Twitter username or skip to switch this feature off\n");
    scanf("%s", username);
    if(strcmp(username, "skip") == 0) {
      return 0;
    }
    
    printf("Enter your Twitter password\n");
    scanf("%s", password);
    
  }
  
  return 0;
  //Check if twitter details have been entered and whether user has switched off
  //being asked for details
  ////If details entered, create new thread
  ////If not details entered and switched off, return
  ////
}
