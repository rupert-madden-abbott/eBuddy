#include "main.h"
#include <pthread.h>

#define NOTI_CONF_FILE "notify.conf"

int noti_initialize(void);
int noti_authenticate(void);
void noti_parse_response(char *response, char **token, char **secret);
char *noti_strdup(const char *str);
