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

int nt_init(qu_queue *queue, const char *config);
int nt_destroy(void);
int nt_authenticate(nt_token app, nt_token *user, const char *config);
int nt_request_token(const char *uri, nt_token app, nt_token *user);
int nt_parse_response(char *response, nt_token *token);
int nt_parse_arg(char *arg, const char *type, char *value);
nt_message *nt_get_tweet(const char *uri, nt_token app, nt_token user);
char *nt_curl_get (const char *uri, const char *query);
size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);
void *nt_poll(void *data);


int nt_isyes(char input);
int nt_isno(char input);
int nt_isans(char input);
int nt_ask(const char *question, char *input);
int nt_flush(void);
int nt_validate_int(char *line);

#endif

