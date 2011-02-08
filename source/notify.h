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

/**
 * Initialises the notification module. First checks if the user has
 * already authenticated. If they haven't, go through the oAuth algorithm.
 * Otherwise, create a new thread and being polling Twitter in that thread.
 *
 * @param queue The queue to be used for communication with the polling thread
 * @param config The path to the location of the configuration file
 * @return UT_ERR_CURL_SETUP if curl setup failed. UT_ERR_JSON_ENCODE if 
 * reading/creating the configuration file failed. 0 on success
 */
int nt_init(qu_Queue *queue, const char *config);

/**
 * Perform cleanup when the notifications module is finished with.
 *
 * @param queue The queue to be destroyed.
 */
void nt_destroy(qu_Queue *queue);

/**
 * Contains the main algorithm for authenticating a user based on the oAuth 
 * method. Acquires a request token and then asks the user to log in to
 * Twitter via a browser. The user then copies the provided PIN number back
 * into the command line and an access token is acquired and then saved to
 * configuration file.
 *
 * @param app The token containing the applications key and secret
 * @param user The token containing to hold both the request and access data
 * @param config The path to the configuration file
 * @param root A JSON object holding the current configuration data
 */
int nt_authenticate(const nt_Token app, nt_Token *user, const char *config, 
                    cf_Json *root);
                    
/**
 * Performs a request to the specified uri. If the response contains a token in 
 * a valid, url encoded format, this data is extracted and returned in the user
 * parameter.
 *
 * @param uri The uri to send the request to
 * @param app The applications token to uniquely identify the application
 * @param user The returned token to uniquely identify the user
 */
int nt_request_token(const char *uri, nt_Token app, nt_Token *user);

/**
 * Parses a response and, if the response is valid and url encoded, returns the 
 * data in the token parameter.
 * 
 * @param response The string containing the response
 * @param token The token in which to store the extracted data
 */
int nt_parse_response(char *response, nt_Token *token);

/**
 * Parses a url encoded argument in the format of name=value. Confirms that the
 * name of the parameter is equivalent to @p name.
 *
 * @param arg The string containing the formatted argument
 * @param name The expected name of the argument
 * @return The value of the argument or NULL on error
 */
char *nt_parse_arg(char *arg, const char *name);

/**
 * Performs a get request to Twitter's friend timeline every 20 seconds. Should
 * be called from pthread_create so that it runs in its own thread. When a new
 * tweet is recieved, the data is written to the location pointed at by @p data
 *
 * @param The location at which recieved data is stored
 * @return The return should not be checked as it is called by pthread
 */
void *nt_poll(void *data);

/**
 * Performs a get request to the specified URI. The request is signed with both
 * the application and user secrets. The response is expected to be in a JSON
 * format and, if valid, is parsed into an nt_Message structure
 *
 * @warning The user must have been authenticated in order to use this function
 *
 * @param uri The uri to send the request to
 * @param app The application's token to uniquely identify the application
 * @param user A valid access token to uniquely identify the user
 * @param tweet The structure in which to store the response data
 * @return The response data on success or NULL on error
 */
nt_Message *nt_get_tweet(const char *uri, nt_Token app, nt_Token user, 
                         nt_Message *tweet);
                         
/**
 * Performs a get request to the specified URI with the specified query string
 *
 * @param uri The uri to send the request to
 * @param query The query string to append to the uri
 * @return The response data formatted as a string on success or NULL on error
 */
char *nt_curl_get (const char *uri, const char *query);

/**
 * A callback function used exclusively by libcurl to recursively create space
 * for the response until there is enough for the entire response
 */
size_t nt_write_response(void *ptr, size_t size, size_t nmemb, void *data);

#endif

