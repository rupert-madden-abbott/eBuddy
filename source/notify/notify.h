#ifndef NOTIFY_H
#define NOTIFY_H

#include "main.h"
#include <pthread.h>
#include <oauth.h>
#include <string.h>

#define NOTI_CONF_FILE "notify.conf"

#define NOTI_TWITTER_KEY "pFQbE7wR2YXfGbp8muYKA"
#define NOTI_TWITTER_SECRET "iam3544JfaQLjS13pO6LjsGWtDtpanZIXrB93vnk4"
#define NOTI_TWITTER_REQUEST "https://api.twitter.com/oauth/request_token"
#define NOTI_TWITTER_ACCESS "https://api.twitter.com/oauth/access_token"
#define NOTI_TWITTER_AUTHORIZE "https://api.twitter.com/oauth/authorize"

typedef struct noti_token {
	char key[50];
	char secret[50];
} noti_token;

int noti_initialize(void);
int noti_authenticate(noti_token *user);
int noti_request_token(char *uri, noti_token app, noti_token *user);
int noti_parse_response(char *response, noti_token *token);
int noti_read_arg(char *arg, char *type, char *value);

#endif

