#include <string.h>
#include <curl/curl.h>
#include <pthread.h>
#include <unistd.h>
#include <oauth.h>
#include <ctype.h>
#include <string.h>
#include <jansson.h>
#include "utility.h"
#include "config.h"
#include "queue.h"
#include "notify.h"

int nt_init(qu_queue *queue, const char *config) {
  int         authenticated, 
              rc;
  nt_arg      *data = { { "", "" }, { "", "" }, queue, config };
  nt_token    user = { "", "" }, 
              app = { "", "" };
  cf_json     *root = NULL;
  pthread_t   thread;

  //Initialize curl  
  rc = curl_global_init(CURL_GLOBAL_ALL);
  if(rc) {
    return UT_ERR_CURL_SETUP;
  }
  
  //Initialize nt_arg
  data->
  

  //Attempt to load the config file or create a default if it cannot be found
  root = cf_read(config);
  if(!root) {
    root = cf_create(NT_CONFIG_DEFAULT, config);    
    if(!root) {
      return UT_ERR_JSON_ENCODE;
    }
  }
  
  //Initialize app key
  rc = strlen(NT_APP_KEY);
  if(rc > NT_KEY_MAX) {
    cf_free(root);
    return UT_ERR_UNKNOWN;
  }
  strcpy(app.key, NT_APP_KEY);

  //Initialize app secret
  rc = strlen(NT_APP_SECRET);
  if(rc > NT_KEY_MAX) {
    cf_free(root);
    return UT_ERR_UNKNOWN;
  }
  strcpy(app.secret, NT_APP_SECRET);
  
  //Check if authentication has already occurred
  authenticated = cf_get_integer(root, "authenticated");
  if(authenticated == 0) {
    //authenticate the user
    rc = nt_authenticate(app, &user, config, root);
    if(rc) {
      cf_free(root);
      return rc;
    }
  }
  
  cf_free(root);

  //Create a new thread to start polling twitter
  pthread_create(&thread, NULL, nt_poll, queue);

  return UT_ERR_NONE;
}

int nt_destroy(qu_queue *queue) {
  qu_free(queue);
  curl_global_cleanup();
  return UT_ERR_NONE;
}

int nt_authenticate(const nt_token app, nt_token *user, const char *config, 
                    cf_json *root) {
  int i, j, rc;
  char url[NT_URL_MAX], pin[NT_KEY_MAX];
  const char *command;
  
  //Get a request token
  rc = nt_request_token(NT_TWITTER_REQUEST, app, user);
  if(rc) {
    return rc;
  }

  //Determine the command to open a browser window, according to the OS  
  if(UT_OS == UT_OS_LINUX) {
    command = "xdg-open";
  }
  else if(UT_OS == UT_OS_OSX) {
    command = "open";
  }
  else {
    return UT_ERR_UNKNOWN;
  }
  
  //Format the url to be opened in a browser
  rc = snprintf(url, NT_URL_MAX, "%s %s?oauth_token=%s", command, 
                NT_TWITTER_AUTHORIZE, user->key);
  if(rc < 0) {
    return UT_ERR_UNKNOWN;
  }
  
  //Tell the user what to do when the browser opens
  printf("In a moment, your browser will open. Please grant access to this "
         "application. You may need to login to your account. Once you have "
         "done so, enter the PIN below\nPIN: ");
  if(rc < 0) {
    return UT_ERR_UNKNOWN;
  }       
  
  //Open a browser with the specified url
  if(system(url)) return UT_ERR_UNKNOWN;
  
  //Keep requesting an access token until one is returned
  j = 0;
  do {
    if(j) {
      //If the user has entered an incorrect value, print an error message
      rc = printf("The PIN you entered was rejected. Please try again: ");
      if(rc) {
        return UT_ERR_UNKNOWN;
      }
    }
    
    //Keep asking the user for input until they enter a whole number
    i = 0;
    do {
      //If the user has entered an incorrect value, print an error message
      if(i) {
        rc = printf("You must enter a whole number. Please try again: ");
        if(rc) {
          return UT_ERR_UNKNOWN;
        }
      }
      
      //Get the user's pin
      fgets(pin, NT_KEY_MAX, stdin);
      rc = ferror(stdin);
      if(rc) {
        return UT_ERR_UNKNOWN;
      }

      //Ensure string is null terminated
      pin[strlen(pin) - 1] = '\0';
      
      i = 1;
    } while(ut_isint(pin));
    
    rc = snprintf(url, NT_URL_MAX, "%s?oauth_verifier=%s", NT_TWITTER_ACCESS, 
                  pin);
    if(rc) {
      return UT_ERR_UNKNOWN;
    }
    
    j = 1;
  } while(nt_request_token(url, app, user));
  
  //Set the user's access token and authentication to true
  rc = cf_set_string(root, "user_key", user->key);
  if(rc) {
    return rc;
  }
  
  rc = cf_set_string(root, "user_secret", user->secret);
  if(rc) {
    return rc;
  }
  
  rc = cf_set_integer(root, "authenticated", 1);
  if(rc) {
    return rc;
  }
  
  //Save the config file
  rc = cf_write(root, config);
  if(rc) {
    return rc;
  }
  
  return UT_ERR_NONE;
}

int nt_request_token(const char *uri, nt_token app, nt_token *user) {
  int rc;
  char *url = NULL, *response = NULL, *postargs;
  
  //Prepare the request with an oauth signature
  url = oauth_sign_url2(uri, &postargs, OA_HMAC, "POST", app.key, 
                        app.secret, user->key, user->secret);
  if(!url) {
    return UT_ERR_OAUTH_SIGN;
  }
  
  //Send request
  response = oauth_http_post2(url, postargs, NULL);
  
  free(postargs);
  free(url);
  
  //Retrieve the token from the response
  rc = nt_parse_response(response, user);
  if(rc) {
    return rc;
  }

  return UT_ERR_NONE;
}

int nt_parse_response(char *response, nt_token *token) {
  int rc;
  char **rv = NULL, *check;
  
  //Get the parameters from the response and ensure there are at least 2
  rc = oauth_split_url_parameters(response, &rv);
  if(rc < 2) {
    return UT_ERR_INVALID_RESPONSE;
  }
  
  //Get the key
  check = nt_parse_arg(rv[0], "oauth_token");
  rc = strlen(check);
  if(rc > NT_KEY_MAX) {
    free(rv);
    return UT_ERR_INVALID_RESPONSE;
  }
  strcpy(token->key, check);
  
  //Get the secret
  check = nt_parse_arg(rv[1], "oauth_token_secret");
  rc = strlen(check);
  free(rv);
  if(rc > NT_KEY_MAX) {
    return UT_ERR_INVALID_RESPONSE;
  }
  strcpy(token->secret, check);

  return UT_ERR_NONE;
}

char *nt_parse_arg(char *arg, const char *name) {
  int rc;
  char *piece;
  
  //Get the parameter name
  piece = strtok(arg, "=");
  if(!piece) {
    return NULL;
  }
  
  //Ensure the name matches the expected name
  rc = strcmp(piece, name);
  if(rc) {
    return NULL;
  }
  
  //Return the value
  return strtok(NULL, "=");
}

void *nt_poll(void *arg) {
  int rc;
  cf_json *root;
  nt_message *tweet; 
  const char *config = "conf/notify.json";
  char last_tweet[NT_ID_MAX], *check;
  nt_token user = { "", "" }, app = { "", "" };

  //Attempt to load the config file
  root = cf_read(config);
  if(!root) {
    return NULL;
  }

  //Extract data from config file
  rc = strlen(NT_APP_KEY);
  if(rc > NT_KEY_MAX) {
    return NULL;
  }
  check = cf_get_nstring(root, "app_key", NT_KEY_MAX);
  if(!check) {
    cf_free(root);
    return NULL;
  }
  strcpy(app.key, check);
  
  check = cf_get_nstring(root, "app_secret", NT_KEY_MAX);
  if(!check) {
    cf_free(root);
    return NULL;
  }
  strcpy(app.secret, check);
  
  check = cf_get_nstring(root, "user_key", NT_KEY_MAX);
  if(!check) {
    cf_free(root);
    return NULL;
  }
  strcpy(user.key, check);
  
  check = cf_get_nstring(root, "user_secret", NT_KEY_MAX);
  if(!check) {
    cf_free(root);
    return NULL;
  }
  strcpy(user.secret, check);
  
  check = cf_get_nstring(root, "last_tweet", NT_KEY_MAX);
  if(!check) {
    cf_free(root);
    return NULL;
  }
  strcpy(last_tweet, check);

      printf("poll\n");
      fflush(stdout);

  //Poll for tweets every 20 seconds
  while(1) {
    tweet = nt_get_tweet(NT_TWITTER_POLL, app, user);
    if(!tweet) {
      printf("No tweet\n");
      fflush(stdout);
      return NULL;
    }
    
    printf("Outside: Last Tweet: %s This Tweet: %s\n\n", last_tweet, tweet->id);
    fflush(stdout);
    if(strcmp(tweet->id, last_tweet)) {
      printf("Inside: Last Tweet: %s This Tweet: %s\n\n", last_tweet, tweet->id);
      strncpy(last_tweet, tweet->id, NT_ID_MAX);
      cf_set_string(root, "last_tweet", tweet->id);
      cf_write(root, config);
      qu_push(queue, tweet);
    }
    sleep(20);
  }
  pthread_exit(NULL);
}

nt_message *nt_get_tweet(const char *uri, nt_token app, nt_token user) {
  nt_message *tweet;
  cf_json *root, *object, *user_object;
  char *url = NULL, *postargs = NULL, *response = NULL, *check;
 
  //Prepare the request with an oauth signature
  url = oauth_sign_url2(uri, &(postargs), OA_HMAC, "GET", app.key, app.secret, 
                        user.key, user.secret);
  if(!url) {
    return NULL;
  }
  
  //Send the request
  response = nt_curl_get(url, postargs);
  free(postargs);
  free(url);
  if(!response) {
    return NULL;  
  }

  //Parse the JSON response
  root = cf_read(response);
  free(response);
  if(!root) {
    return NULL;
  }
  
  //Get the array of tweets
  object = cf_get_array(root, 0);
  if(!object) {
    cf_free(root);
    return NULL;
  }

  //Make space for the tweet
  tweet = (nt_message *)malloc(sizeof(nt_message));
  if(!tweet) {
    cf_free(root);
    return NULL;
  } 
  
  //Set the application to twitter
  strncpy(tweet->app, "twitter", NT_APP_MAX);
  
  //Get the text of the tweet
  check = cf_get_nstring(object, "text", NT_TEXT_MAX);
  if(!check) {
    cf_free(root);
    free(tweet);
    return NULL;
  }
  strcpy(tweet->text, check);
  
  //Get the users screen name
  user_object = cf_get_object(object, "user");
  if(!user_object) {
    cf_free(root);
    free(tweet);
    return NULL;
  }
  
  check = cf_get_nstring(user_object, "screen_name", NT_USER_MAX);
  if(!check) {
    cf_free(root);
    free(tweet);
    return NULL;
  }
  strcpy(tweet->user, check);
  
  //Get the tweets ID
  check = cf_get_nstring(object, "id_str", NT_ID_MAX);
  if(!check) {
    cf_free(root);
    free(tweet);
    return NULL;
  }
    
  return tweet;
}

char *nt_curl_get (const char *uri, const char *query) {
  int rc;
  CURL *curl;
  char *url = NULL;
  nt_response chunk = { NULL, 0 };

  //Create space for the url  
  url = (char *)malloc(sizeof(char) * (strlen(uri) + strlen(query) + 2));
  if(!url) {
    return NULL;
  }
  
  //Create the url from the uri and the query string
  rc = sprintf(url, "%s?%s", uri, query);
  if(rc) {
    free(url);
    return NULL;
  }
  
  //Initialise curl
  curl = curl_easy_init();
  if(!curl) {
    free(url);
    return NULL;
  }
  
  //Set the url
  rc = curl_easy_setopt(curl, CURLOPT_URL, url);
  free(url);
  if(rc) {

    curl_easy_cleanup(curl);
    return NULL;
  }
  
  //Set the location for the response to be written to
  rc = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  if(rc) {
    curl_easy_cleanup(curl);
    return NULL;
  }

  //Set the callback function for writing the response
  rc = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nt_write_response);
  if(rc) {
    curl_easy_cleanup(curl);
    return NULL;
  }
  
  //Send the request
  rc = curl_easy_perform(curl);
  if(rc) {
    curl_easy_cleanup(curl);
    return NULL;
  }
  
  curl_easy_cleanup(curl);
  
  return chunk.data;
}

size_t nt_write_response(void *ptr, size_t size, size_t nmemb, void *data) {
  size_t real_size = size * nmemb;
  nt_response *mem = (nt_response *)data;

  //Reallocate the space if the response is too large
  mem->data = (char *)realloc(mem->data, mem->size + real_size + 1);
  if(mem->data) {
    //Reinitialize nt_response
    memcpy(&(mem->data[mem->size]), ptr, real_size);
    mem->size += real_size;
    mem->data[mem->size] = 0;
  }
  return real_size;
}
