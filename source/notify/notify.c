#include "notify.h"

int nt_init(nt_node *queue, const char *config);
  char input = '\0', *config = "conf/notify.json";
  nt_token user = { "", "" }, app = { "", "" };
  cf_json *root = NULL;
  int authenticated, disabled;
  pthread_t thread;
  
  curl_global_init(CURL_GLOBAL_ALL);
  
  root = cf_read(config);
  if(!root) return err_unknown;
  
  disabled = cf_get_integer(root, "disabled");
  if(disabled) return err_none;

  authenticated = cf_get_integer(root, "authenticated");
  strncpy(app.key, cf_get_string(root, "app_key"), NT_KEY_MAX);
  if(!app.key) return err_unknown;
  strncpy(app.secret, cf_get_string(root, "app_secret"), NT_KEY_MAX);
  if(!app.secret) return err_unknown;
  
  if(authenticated) {
    strncpy(user.key, cf_get_string(root, "user_key"), NT_KEY_MAX);
    if(!user.key) return err_unknown;
    strncpy(user.secret, cf_get_string(root, "user_secret"), NT_KEY_MAX);
    if(!user.secret) return err_none;
  }
  else { 
    if(nt_ask("Would you like to setup Twitter now?", &input)) return err_unknown;
    if(nt_isno(input)) {
      input = '\0';
      if(nt_ask("Would you like to disable the Twitter feature?", &input)) return err_unknown;
      if(nt_isyes(input)) {
        if(cf_set_integer(root, "disabled", 1)) return err_unknown;
        if(cf_write(root, config)) return err_unknown;
        printf("Twitter has been disabled\n");
        return err_none;
      }
      else if(nt_isno(input)) {
        printf("You will have the opportunity to setup Twitter when you next "
               "start your eBuddy\n");
        return err_none;
      }
      else return err_unknown;
    }
    else if(nt_isyes(input)) {
      printf("Setting up Twitter...\n");
      if(nt_authenticate(app, &user, config)) return err_unknown;
    }
    else return err_unknown;
  }

  pthread_create(&thread, NULL, nt_poll, NULL);  

  return err_none;
}

int nt_destroy() {
  curl_global_cleanup();
  return err_none;
}

void *nt_poll(void *data) {
  cf_json *root;
  nt_message tweet; 
  char *config = "conf/notify.json";
  nt_token user = { "", "" }, app = { "", "" };
  
  root = cf_read(config);
  
  strncpy(app.key, cf_get_string(root, "app_key"), NT_KEY_MAX);
  strncpy(app.secret, cf_get_string(root, "app_secret"), NT_KEY_MAX);
  strncpy(user.key, cf_get_string(root, "user_key"), NT_KEY_MAX);
  strncpy(user.secret, cf_get_string(root, "user_secret"), NT_KEY_MAX);

  while(1) {
    sleep(5);
    tweet = nt_get_tweet("http://api.twitter.com/1/statuses/friends_timeline.json?count=1", app, user);
    
    printf("USER: %s\nID: %s\nTEXT: %s\n\n", tweet.user, tweet.id, tweet.text);
  }
  pthread_exit(NULL);
}

int nt_authenticate(nt_token app, nt_token *user, char *config) {
  int i, j;
  char url[1000], pin[NT_KEY_MAX];
  cf_json *root;
  
  //Get request token
  if(nt_request_token(NT_TWITTER_REQUEST, app, user)) return err_unknown;

  //Get authorization pin
  if(OS == LINUX) {
    sprintf(url, "xdg-open %s?oauth_token=%s", NT_TWITTER_AUTHORIZE, 
            user->key);
  }
  else if(OS == OSX) {
    sprintf(url, "open %s?oauth_token=%s", NT_TWITTER_AUTHORIZE, user->key);
  }
  else return err_unknown;
  
  printf("In a moment, your browser will open. Please grant access to this "
         "application. You may need to login to your account. Once you have "
         "done so, enter the PIN below\nPIN: ");
  if(system(url)) return err_unknown;
  
  j = 0;
  do {
    if(j) {
      printf("The PIN you entered was rejected. Please try again: ");
    }
    i = 0;
    do {
      if(i) {
        printf("You must enter a whole number. Please try again: ");
      }
      fgets(pin, NT_KEY_MAX, stdin);
      pin[strlen(pin) - 1] = '\0';
      i = 1;
    } while(nt_validate_int(pin));
    
    sprintf(url, "%s?oauth_verifier=%s", NT_TWITTER_ACCESS, pin);
    j = 1;
  } while(nt_request_token(url, app, user));
  
  root = cf_read(config);

  if(cf_set_string(root, "user_key", user->key)) return err_unknown;
  if(cf_set_string(root, "user_secret", user->secret)) return err_unknown;
  if(cf_set_integer(root, "authenticated", 1)) return err_unknown;

  if(cf_write(root, config)) return err_unknown;
  
  cf_free(root);
  
  return err_none;
}

int nt_request_token(char *uri, nt_token app, nt_token *user) {
  char *url = NULL, *response = NULL, *postargs;
  
  url = oauth_sign_url2(uri, &postargs, OA_HMAC, "POST", app.key, 
                        app.secret, user->key, user->secret);
  response = oauth_http_post(url, postargs);
  if(postargs) free(postargs);
  if(url) free(url);
  if(nt_parse_response(response, user)) return err_unknown;
  return err_none;
}

int nt_parse_response(char *response, nt_token *token) {
  char **rv = NULL, key[70], secret[70];
  
  if(oauth_split_url_parameters(response, &rv) < 2) return err_unknown;
  strcpy(key, rv[0]);
  strcpy(secret, rv[1]);
  free(rv);
  
  if(nt_parse_arg(key, "oauth_token", token->key)) return err_unknown;
  if(nt_parse_arg(secret, "oauth_token_secret", token->secret)) return err_unknown;
  
  return err_none;
}

int nt_parse_arg(char *arg, char *type, char *value) {
  char *piece;
  
  piece = strtok(arg, "=");
  if(!piece) return err_unknown;
  if(strcmp(piece, type)) return err_unknown;
  piece = strtok(NULL, "=");
  if(!piece) return err_unknown;
  memcpy(value, piece, 50 * sizeof(char));
  
  return err_none;
}

nt_message nt_get_tweet(char *uri, nt_token app, nt_token user) {
  nt_message tweet;
  cf_json *root, *object;
  char *url = NULL, *postargs = NULL, *response = NULL;
  url = oauth_sign_url2(uri, &(postargs), OA_HMAC, "GET", app.key, app.secret, 
                        user.key, user.secret);
  
  response = nt_curl_get(url, postargs);

  if(postargs) free(postargs);
  if(url) free(url);
  
  root = cf_read(response);

  if(response) free(response);
  
  object = (cf *)json_array_get((const json_t *)root, 0);
  
  strncpy(tweet.app, "twitter", NT_APP_MAX);
  strncpy(tweet.text, cf_get_string(object, "text"), NT_TEXT_MAX);
  strncpy(tweet.user, cf_get_string(cf_get_object(object, "user"), "screen_name"), NT_USER_MAX);
  strncpy(tweet.id, cf_get_string(object, "id_str"), NT_ID_MAX);  
    
  return tweet;
}

char *nt_curl_get (const char *uri, const char *query) {
  CURL *curl;
  char *url = NULL;
  struct MemoryStruct chunk;
  
  if(!query) return NULL;
  
  url = (char *)malloc(sizeof(char) * (strlen(uri) + strlen(query) + 2));
  sprintf(url, "%s?%s", uri, query);
  
  chunk.data = NULL;
  chunk.size = 0;
  
  curl = curl_easy_init();
  if(!curl) return NULL;
  
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_perform(curl);
  
  curl_easy_cleanup(curl);
  
  if(url) free(url);
  
  return chunk.data;
}

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)data;

  mem->data = (char *)realloc(mem->data, mem->size + realsize + 1);
  if (mem->data) {
    memcpy(&(mem->data[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
  }
  return realsize;
}

int nt_validate_int(char *line) {
  unsigned int i;
  if(strlen(line) == 0) return err_unknown;

  for(i = 0; i < strlen(line); i++) {
    if(!isdigit((int) line[i])) return err_unknown;
  }
  return err_none;  
}

int nt_flush(void) {
  int c;
  
  while ((c = getchar()) != '\n' && c != EOF);
  
  return err_none;
}

int nt_isyes(char input) {
  if(input == 'y' || input == 'Y') return err_unknown;
  else return err_none;
}

int nt_isno(char input) {
  if(input == 'n' || input == 'N') return err_unknown;
  else return err_none;
}

int nt_isans(char input) {
  if(nt_isyes(input) || nt_isno(input)) return err_unknown;
  else return err_none;
}

int nt_ask(char *question, char *input) {
  int i = 0;
  
  printf("%s (y/n)\n", question);
  do {
    if(i) {
      printf("Please enter y or n: ");
    }
    *input = getchar();
    if(nt_flush()) return err_unknown;
    i = 1;
  } while(!nt_isans(*input));
  
  return err_none;
}

