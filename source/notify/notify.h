#ifndef NOTIFY_H
#define NOTIFY_H

#include <string.h>
#include <curl/curl.h>
#include <pthread.h>
#include <unistd.h>
#include <oauth.h>
#include <ctype.h>
#include "config.h"

#define NT_CONF_FILE "notify.conf"

#define NT_TWITTER_REQUEST "https://api.twitter.com/oauth/request_token"
#define NT_TWITTER_ACCESS "https://api.twitter.com/oauth/access_token"
#define NT_TWITTER_AUTHORIZE "https://api.twitter.com/oauth/authorize"

#define NT_KEY_MAX 200
#define NT_TEXT_MAX 141
#define NT_USER_MAX 16
#define NT_ID_MAX 21
#define NT_APP_MAX 50


typedef struct nt_token {
	char key[NT_KEY_MAX];
	char secret[NT_KEY_MAX];
} nt_token;

struct MemoryStruct {
  char *data;
  size_t size; //< bytes remaining (r), bytes accumulated (w)
};

typedef struct nt_message {
  char app[NT_APP_MAX];
  char text[NT_TEXT_MAX];
  char user[NT_USER_MAX];
  char id[NT_ID_MAX];
} nt_message;

typedef struct nt_node {
  nt_message message;
  struct nt_node *next;
} nt_node;

int nt_init(nt_node *queue, const char *config);
int nt_destroy(void);
int nt_authenticate(nt_token app, nt_token *user, const char *config);
int nt_request_token(char *uri, nt_token app, nt_token *user);
int nt_parse_response(char *response, nt_token *token);
int nt_parse_arg(char *arg, char *type, char *value);
nt_message nt_get_tweet(char *uri, nt_token app, nt_token user);
char *nt_curl_get (const char *uri, const char *query);
size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);
void *nt_poll(void *data);


int nt_isyes(char input);
int nt_isno(char input);
int nt_isans(char input);
int nt_ask(char *question, char *input);
int nt_flush(void);
int nt_validate_int(char *line);

#endif

