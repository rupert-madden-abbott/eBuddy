#ifndef NOTIFY_H
#define NOTIFY_H

#include "main.h"
#include "config.h"
#include <pthread.h>
#include <oauth.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>

#define NOTI_CONF_FILE "notify.conf"

#define NOTI_TWITTER_REQUEST "https://api.twitter.com/oauth/request_token"
#define NOTI_TWITTER_ACCESS "https://api.twitter.com/oauth/access_token"
#define NOTI_TWITTER_AUTHORIZE "https://api.twitter.com/oauth/authorize"

#define NOTI_MAX 200

typedef struct noti_token {
	char key[NOTI_MAX];
	char secret[NOTI_MAX];
} noti_token;

struct MemoryStruct {
  char *data;
  size_t size; //< bytes remaining (r), bytes accumulated (w)
};

int noti_initialize(void);
int noti_destroy(void);
int noti_authenticate(noti_token app, noti_token *user, char *config);
int noti_request_token(char *uri, noti_token app, noti_token *user);
int noti_parse_response(char *response, noti_token *token);
int noti_parse_arg(char *arg, char *type, char *value);
int noti_poll(char *uri, noti_token app, noti_token user);
char *noti_curl_get (const char *uri, const char *query);
size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);


int noti_isyes(char input);
int noti_isno(char input);
int noti_isans(char input);
int noti_ask(char *question, char *input);
int noti_flush(void);
int noti_validate_int(char *line);

#endif

