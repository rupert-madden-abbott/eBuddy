#include "notify.h"

int noti_initialize(void) {
  char input = '\0', *config = "conf/notify.json";
  noti_token user = { "", "" }, app = { "", "" };
  json_t *root = NULL;
  int authenticated, disabled;
  
  curl_global_init(CURL_GLOBAL_ALL);
  
  if(conf_load(config, &root)) return 1;
  
  if(conf_readi(root, "disabled", &disabled)) return 1;
  if(disabled) return 0;

  if(conf_readi(root, "authenticated", &authenticated)) return 1;
  if(conf_reads(root, "app_key", app.key)) return 1;
  if(conf_reads(root, "app_secret", app.secret)) return 1;

  if(authenticated) {
    if(conf_reads(root, "user_key", user.key)) return 1;
    if(conf_reads(root, "user_secret", user.secret)) return 1;
  }
  else { 
    if(noti_ask("Would you like to setup Twitter now?", &input)) return 1;
    if(noti_isno(input)) {
      input = '\0';
      if(noti_ask("Would you like to disable the Twitter feature?", &input)) return 1;
      if(noti_isyes(input)) {
        if(conf_updatei(&root, "disabled", 1)) return 1;
        if(conf_write(config, root)) return 1;
        printf("Twitter has been disabled\n");
        return 0;
      }
      else if(noti_isno(input)) {
        printf("You will have the opportunity to setup Twitter when you next "
               "start your eBuddy\n");
        return 0;
      }
      else return 1;
    }
    else if(noti_isyes(input)) {
      printf("Setting up Twitter...\n");
      if(noti_authenticate(app, &user, config)) return 1;
    }
    else return 1;
  }
  
  json_decref(root);

  if(noti_poll("http://api.twitter.com/1/statuses/user_timeline.json?count=1", app, user)) return 1;
  
  
  
  return 0;
}

int noti_destroy() {
  curl_global_cleanup();
  return 0;
}

int noti_authenticate(noti_token app, noti_token *user, char *config) {
  int i, j;
  char url[1000], pin[NOTI_MAX];
  json_t *root = NULL;

  //Get request token
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
      fgets(pin, NOTI_MAX, stdin);
      pin[strlen(pin) - 1] = '\0';
      i = 1;
    } while(noti_validate_int(pin));
    
    sprintf(url, "%s?oauth_verifier=%s", NOTI_TWITTER_ACCESS, pin);
    j = 1;
  } while(noti_request_token(url, app, user));

  if(conf_load(config, &root)) return 1;

  if(conf_updates(&root, "user_key", user->key)) return 1;
  if(conf_updates(&root, "user_secret", user->secret)) return 1;
  if(conf_updatei(&root, "authenticated", 1)) return 1;

  if(conf_write(config, root)) return 1;
  
  return 0;
}

int noti_request_token(char *uri, noti_token app, noti_token *user) {
  char *url = NULL, *response = NULL, *postargs;
  
  url = oauth_sign_url2(uri, &postargs, OA_HMAC, "POST", app.key, 
                        app.secret, user->key, user->secret);
  response = oauth_http_post(url, postargs);
  if(postargs) free(postargs);
  if(url) free(url);
  if(noti_parse_response(response, user)) return 1;
  return 0;
}

int noti_parse_response(char *response, noti_token *token) {
  char **rv = NULL, key[70], secret[70];
  
  if(oauth_split_url_parameters(response, &rv) < 2) return 1;
  strcpy(key, rv[0]);
  strcpy(secret, rv[1]);
  free(rv);
  
  if(noti_parse_arg(key, "oauth_token", token->key)) return 1;
  if(noti_parse_arg(secret, "oauth_token_secret", token->secret)) return 1;

  printf("\n\nUser Key (%i): %s\nUser Secret(%i): %s\n\n", strlen(token->key), token->key, strlen(token->secret), token->secret);
  
  return 0;
}

int noti_parse_arg(char *arg, char *type, char *value) {
  char *piece;
  
  piece = strtok(arg, "=");
  if(!piece) return 1;
  if(strcmp(piece, type)) return 1;
  piece = strtok(NULL, "=");
  if(!piece) return 1;
  memcpy(value, piece, 50 * sizeof(char));
  
  return 0;
}

int noti_poll(char *uri, noti_token app, noti_token user) {
  char *url = NULL, *postargs = NULL, *response = NULL;
  url = oauth_sign_url2(uri, &(postargs), OA_HMAC, "GET", app.key, app.secret, 
                        user.key, user.secret);
  
  printf("\n\n%s %s\n\n", url, postargs);
  
  response = noti_curl_get(url, postargs);

  if(postargs) free(postargs);
  if(url) free(url);
  
  printf("%s", response);
  
  if(response) free(response);
  
  return 0;
}

char *noti_curl_get (const char *uri, const char *query) {
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

int noti_validate_int(char *line) {
  unsigned int i;
  if(strlen(line) == 0) return 1;

  for(i = 0; i < strlen(line); i++) {
    if(!isdigit((int) line[i])) return 1;
  }
  return 0;  
}

int noti_flush(void) {
  int c;
  
  while ((c = getchar()) != '\n' && c != EOF);
  
  return 0;
}

int noti_isyes(char input) {
  if(input == 'y' || input == 'Y') return 1;
  else return 0;
}

int noti_isno(char input) {
  if(input == 'n' || input == 'N') return 1;
  else return 0;
}

int noti_isans(char input) {
  if(noti_isyes(input) || noti_isno(input)) return 1;
  else return 0;
}

int noti_ask(char *question, char *input) {
  int i = 0;
  
  printf("%s (y/n)\n", question);
  do {
    if(i) {
      printf("Please enter y or n: ");
    }
    *input = getchar();
    if(noti_flush()) return 1;
    i = 1;
  } while(!noti_isans(*input));
  
  return 0;
}

