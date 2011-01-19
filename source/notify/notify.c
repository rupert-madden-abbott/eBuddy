#include "notify.h"
#include "config.h"
#include <oauth.h>
#include <string.h>

int noti_initialize(void) {
  noti_authenticate();
   
  /* printf("Enter your Twitter username or skip to switch this feature off\n");
    scanf("%s", username);
    if(strcmp(username, "skip") == 0) {
      return 0;
    }
    
    printf("Enter your Twitter password\n");
    scanf("%s", password);
    */
  
  return 0;
  //Check if twitter details have been entered and whether user has switched off
  //being asked for details
  ////If details entered, create new thread
  ////If not details entered and switched off, return
  ////
}

int noti_authenticate(void) {
  char *response = NULL;
  noti_request request;

  //Get request token 
  request.uri = NOTI_TWITTER_REQUEST;
  request.url = NULL;
  request.postargs = NULL;
  strcpy(request.app.key, NOTI_TWITTER_KEY);
  strcpy(request.app.secret, NOTI_TWITTER_SECRET);
  
  noti_send_request(&request, response);
  
  noti_parse_response(response, request.user.key, request.user.secret); 
  
/*
  //Get authorization
  sprintf(request.url, "xdg-open %s?oauth_token=%s", NOTI_TWITTER_AUTHORIZE, request.user.key);

  printf("In a moment, your browser will open. Please grant access to this application. You may need to login to your account. Once you have done so, enter the PIN below\n");
  //system(request.url);
  //scanf("%s", request.pin);
  request.pin = "5555";
  //Get access token
  request.uri = (char *)malloc(1000 * sizeof(char));
  sprintf(request.uri, "%s?oauth_verifier=%s", NOTI_TWITTER_ACCESS, request.pin);
  noti_send_request(&request, &response);
  noti_parse_response(response, &(request.user.key), &request.user.secret);

  free(request.uri);*/
  return 0;
}
 
int noti_send_request(noti_request *request, char *response) {
  request->url = oauth_sign_url2(request->uri, &(request->postargs), OA_HMAC, 
                                 NULL, request->app.key, request->app.secret, 
                                 request->user.key, request->user.secret);
                                 
  &response = oauth_http_post(request->url, request->postargs);
  
  return 0;
}

int noti_parse_response(char *response, char *token, char *secret) {
  char **response_parts = NULL;
  
  oauth_split_url_parameters(response, &response_parts);
  
  /**token = "hello";
  noti_strdup(&(response_parts[0][12]));
  *secret = noti_strdup(&(response_parts[1][19]));
  
  free(response_parts);*/
  
  return 0;
}

char *noti_strdup(const char *str) {
  int n = strlen(str) + 1;
  char *dup = malloc(n);
  if(dup) {
      strcpy(dup, str);
  }
  return dup;
}
