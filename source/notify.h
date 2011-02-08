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

/**
 * @define NT_APP_KEY
 * The application key uniquely identifying the eBuddy to Twitter
 */
#define NT_APP_KEY "pFQbE7wR2YXfGbp8muYKA"

/**
 * @define NT_APP_SECRET
 * The application secret used to sign all requests sent to Twitter
 */
#define NT_APP_SECRET "iam3544JfaQLjS13pO6LjsGWtDtpanZIXrB93vnk4"

///{
/**
 * Various URLS for both authenticating a user on Twitter and then accessing
 * Twitter data
 */
#define NT_TWITTER_REQUEST "https://api.twitter.com/oauth/request_token"
#define NT_TWITTER_ACCESS "https://api.twitter.com/oauth/access_token"
#define NT_TWITTER_AUTHORIZE "https://api.twitter.com/oauth/authorize"
#define NT_TWITTER_POLL "http://api.twitter.com/1/statuses/friends_timeline" \
                        ".json?count=1"
///}

/**
 * @define NT_CONFIG_DEFAULT
 * A default JSON string to create the initial configuration file
 */
#define NT_CONFIG_DEFAULT "{\"authenticated\": 0, \"last_tweet\": \"default\"}"

#define NT_KEY_MAX 200
#define NT_TEXT_MAX 141
#define NT_USER_MAX 16
#define NT_ID_MAX 21
#define NT_APP_MAX 50
#define NT_URL_MAX 265

/**
 * @typedef nt_Token
 * A structure to hold an oAuth token
 *
 * @member key The token's key
 * @member secret The token's secret
 */
typedef struct nt_token {
	char key[NT_KEY_MAX];
	char secret[NT_KEY_MAX];
} nt_Token;

/**
 * @typedef nt_Response
 * A structure to hold the response of a request to a URL. This is used by the
 * callback function nt_write_response
 * 
 * @member data The current text of the response
 * @member size The amount of text remaining in the response
 */
typedef struct nt_response {
  char *data;
  size_t size;
} nt_Response;

/**
 * @typedef nt_Message
 * A structure to be pushed onto a queue node which holds the data of a any
 * new tweets.
 *
 * @member error Used to signal main that it should shut down
 * @member app The name of the application sending the notification
 * @member text The content of the notification
 * @member user The name of the user sending the notification
 * @member id The id of the notification, used to ensure it is sent only once
 */
typedef struct nt_message {
  int  error;
  char app[NT_APP_MAX];
  char text[NT_TEXT_MAX];
  char user[NT_USER_MAX];
  char id[NT_ID_MAX];
} nt_Message;

int nt_init(qu_Queue *queue, const char *config);
int nt_destroy(qu_Queue *queue);
int nt_authenticate(const nt_Token app, nt_Token *user, const char *config, 
                    cf_Json *root);
int nt_request_token(const char *uri, nt_Token app, nt_Token *user);
int nt_parse_response(char *response, nt_Token *token);
char *nt_parse_arg(char *arg, const char *name);
nt_Message *nt_get_tweet(const char *uri, nt_Token app, nt_Token user, 
                         nt_Message *tweet);
char *nt_curl_get (const char *uri, const char *query);
size_t nt_write_response(void *ptr, size_t size, size_t nmemb, void *data);
void *nt_poll(void *data);

#endif

