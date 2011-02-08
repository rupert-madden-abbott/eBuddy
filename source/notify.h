#ifndef NOTIFY_H
#define NOTIFY_H

#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#include <curl/curl.h>
#include <oauth.h>

#include "utility.h"
#include "queue.h"
#include "config.h"

#define NT_APP_KEY "pFQbE7wR2YXfGbp8muYKA"
#define NT_APP_SECRET "iam3544JfaQLjS13pO6LjsGWtDtpanZIXrB93vnk4"
#define NT_TWITTER_REQUEST "https://api.twitter.com/oauth/request_token"
#define NT_TWITTER_ACCESS "https://api.twitter.com/oauth/access_token"
#define NT_TWITTER_AUTHORIZE "https://api.twitter.com/oauth/authorize"
#define NT_TWITTER_POLL "http://api.twitter.com/1/statuses/friends_timeline" \
                        ".json?count=1"

#define NT_CONFIG_DEFAULT "{\"authenticated\": 0, \"last_tweet\": \"default\"}"

#define NT_KEY_MAX 200
#define NT_TEXT_MAX 141
#define NT_USER_MAX 16
#define NT_ID_MAX 21
#define NT_APP_MAX 50
#define NT_URL_MAX 265

typedef struct nt_token {
	char key[NT_KEY_MAX];
	char secret[NT_KEY_MAX];
} nt_token;

typedef struct nt_response {
  char *data;
  size_t size;
} nt_response;

typedef struct nt_message {
  int  error;
  char app[NT_APP_MAX];
  char text[NT_TEXT_MAX];
  char user[NT_USER_MAX];
  char id[NT_ID_MAX];
} nt_message;

typedef struct nt_arg {
  nt_token app;
  nt_token user;
  qu_Queue *queue;
  const char *config;
} nt_arg;

int nt_init(qu_Queue *queue, const char *config);
int nt_destroy(qu_Queue *queue);
int nt_authenticate(const nt_token app, nt_token *user, const char *config, 
                    cf_json *root);
int nt_request_token(const char *uri, nt_token app, nt_token *user);
int nt_parse_response(char *response, nt_token *token);
char *nt_parse_arg(char *arg, const char *name);
nt_message *nt_get_tweet(const char *uri, nt_token app, nt_token user, 
                         nt_message *tweet);
char *nt_curl_get (const char *uri, const char *query);
size_t nt_write_response(void *ptr, size_t size, size_t nmemb, void *data);
void *nt_poll(void *data);

#endif

