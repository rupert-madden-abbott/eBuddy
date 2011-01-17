#include "notify.h"
#include "config.h"
#include <oauth.h>
#include <string.h>

int noti_initialize(void) {
  int ask_details, have_details;
  //char username[100], password[100];
  
  ask_details = atoi(conf_read(NOTI_CONF_FILE, "twitter", "ask_details"));
  have_details = atoi(conf_read(NOTI_CONF_FILE, "twitter", "have_details"));
  
  if(!have_details && ask_details) {
    noti_authenticate();
   
   /* printf("Enter your Twitter username or skip to switch this feature off\n");
    scanf("%s", username);
    if(strcmp(username, "skip") == 0) {
      return 0;
    }
    
    printf("Enter your Twitter password\n");
    scanf("%s", password);
    */
  }
  
  return 0;
  //Check if twitter details have been entered and whether user has switched off
  //being asked for details
  ////If details entered, create new thread
  ////If not details entered and switched off, return
  ////
}

int noti_authenticate(void) {
  const char *request_uri = conf_read(NOTI_CONF_FILE, "twitter", "request_uri"),
             *access_uri = conf_read(NOTI_CONF_FILE, "twitter", "access_uri"),
             *authorize_uri = conf_read(NOTI_CONF_FILE, "twitter", "authorize_uri"),
             *consumer_key = conf_read(NOTI_CONF_FILE, "twitter", "consumer_key"),
             *consumer_secret = conf_read(NOTI_CONF_FILE, "twitter", "consumer_secret");
  char *request_url = NULL, *response = NULL, *oauth_token = NULL, pin[50],
       *oauth_secret = NULL, oauth_uri[100], *postargs = NULL;
  
  //Get request token
  printf("Step 1: Get an access token:\n\n");
  
  request_url = oauth_sign_url2(request_uri, NULL, OA_HMAC, NULL, consumer_key, consumer_secret, NULL, NULL);
  printf("Request URL: %s\n\n", request_url);
 
  response = oauth_http_get(request_url, NULL);
  printf("Request response: %s\n\n", response);
 
  noti_parse_response(response, &oauth_token, &oauth_secret); 
  printf("Request Token: %s\nRequest Secret: %s\n\n", oauth_token, oauth_secret);
  
  //Get authorization
  printf("Step 2: Get an authorization PIN:\n\n");
  sprintf(request_url, "xdg-open %s?oauth_token=%s", authorize_uri, oauth_token);
  printf("Authorize URL: %s\n\n", request_url);
  
  printf("In a moment, your browser will open. Please grant access to this application. You may need to login to your account. Once you have done so, enter the PIN below\n");
  system(request_url);
  scanf("%s", pin);
  printf("\nReceived PIN: %s\n\n", pin);
  
  //Get access token
  printf("Step 3: Get an access token:\n\n");
  
  sprintf(oauth_uri, "%s?oauth_verifier=%s", access_uri, pin);
  
  request_url = oauth_sign_url2(oauth_uri, NULL, OA_HMAC, NULL, consumer_key, consumer_secret, oauth_token, oauth_secret);

  printf("Request URL: %s\n\n", request_url);
  
  response = oauth_http_get(request_url, NULL);
  printf("Request response: %s\n\n", response);
  
  noti_parse_response(response, &oauth_token, &oauth_secret);
  printf("Access Token: %s\nRequest Secret: %s\n\n", oauth_token, oauth_secret);
  
  //Update Twitter status
  printf("Step 4: Update Twitter status:\n\n");
  request_url = oauth_sign_url2("http://api.twitter.com/1/statuses/update.json&status=test", &postargs, OA_HMAC, NULL, consumer_key, consumer_secret, oauth_token, oauth_secret);
  printf("Request URL: %s\n\n", request_url);
  
  response = oauth_http_post(request_url, postargs);
  printf("Request response: %s\n\n", response);
  return 1;
}

void noti_parse_response(char *response, char **token, char **secret) {
  char **response_parts = NULL;
  
  oauth_split_url_parameters(response, &response_parts);
  
  *token = noti_strdup(&(response_parts[0][12]));
  *secret = noti_strdup(&(response_parts[1][19]));
  
  free(response_parts);
}

char *noti_strdup(const char *str) {
  int n = strlen(str) + 1;
  char *dup = malloc(n);
  if(dup) {
      strcpy(dup, str);
  }
  return dup;
}
