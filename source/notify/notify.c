#include "notify.h"

int noti_initialize(void) {
  noti_token user;
  json_t *output = NULL;
  
  conf_load("conf/notify.json", output);
  
  //if(noti_authenticate(&user)) return 1;
  

    
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

int noti_authenticate(noti_token *user) {
  char url[1000], pin[200];
  noti_token app;

  //Get request token
  strcpy(app.key, NOTI_TWITTER_KEY);
  strcpy(app.secret, NOTI_TWITTER_SECRET);
  strcpy(user->key, "\0");
  strcpy(user->secret, "\0");
  if(noti_request_token(NOTI_TWITTER_REQUEST, app, user)) return 1;


  //Get authorization pin
  if(OS == LINUX) {
    sprintf(url, "xdg-open %s?oauth_token=%s", NOTI_TWITTER_AUTHORIZE, 
            user->key);
  }
  else if(OS == OSX) {
    sprintf(url, "open %s?oauth_token=%s", NOTI_TWITTER_AUTHORIZE, user->key);
  }
  else return 1;
  
  printf("In a moment, your browser will open. Please grant access to this "
         "application. You may need to login to your account. Once you have "
         "done so, enter the PIN below\nPIN: ");
  if(system(url)) return 1;
  scanf("%s", pin);
  sprintf(url, "%s?oauth_verifier=%s", NOTI_TWITTER_ACCESS, pin);

  //Get access token
  if(noti_request_token(url, app, user)) return 1;

  return 0;
}

int noti_request_token(char *uri, noti_token app, noti_token *user) {
  char *url = NULL, *response = NULL, *postargs;
  
  url = oauth_sign_url2(uri, &postargs, OA_HMAC, "POST", app.key, 
                        app.secret, user->key, user->secret);
  response = oauth_http_post(url, postargs);
  if(postargs) free(postargs);
  if(noti_parse_response(response, user)) return 1;
  return 0;
}

/*
int noti_send_request(noti_request request, char **response) {
  char *url;
  url = oauth_sign_url2(request.url, &(request.postargs), OA_HMAC, NULL, 
                        request.app.key, request.app.secret, 
                        NULL, NULL);
                       
  *response = oauth_http_post(request.url, request.postargs);
  return 0;
}*/

int noti_parse_response(char *response, noti_token *token) {
  char **rv = NULL, key[70], secret[70];
  
  if(oauth_split_url_parameters(response, &rv) < 2) return 1;
  strcpy(key, rv[0]);
  strcpy(secret, rv[1]);
  free(rv);
  
  if(noti_read_arg(key, "oauth_token", token->key)) return 1;
  if(noti_read_arg(secret, "oauth_token_secret", token->secret)) return 1;
  
  return 0;
}

int noti_read_arg(char *arg, char *type, char *value) {
  char *piece;
  
  piece = strtok(arg, "=");
  if(!piece) return 1;
  if(strcmp(piece, type)) return 1;
  piece = strtok(NULL, "=");
  if(!piece) return 1;
  strcpy(value, piece);
  
  return 0;
}


