#include <pthread.h>

#define NOTI_CONF_FILE "notify.conf"

#define NOTI_TWITTER_KEY pFQbE7wR2YXfGbp8muYKA
#define NOTI_TWITTER_SECRET iam3544JfaQLjS13pO6LjsGWtDtpanZIXrB93vnk4
#define NOTI_TWITTER_REQUEST https://api.twitter.com/oauth/request_token
#define NOTI_TWITTER_ACCESS https://api.twitter.com/oauth/access_token
#define NOTI_TWITTER_AUTHORIZE https://api.twitter.com/oauth/authorize

int noti_initialize(void);
int noti_authenticate(void);
void noti_parse_response(char *response, char **token, char **secret);
char *noti_strdup(const char *str);
