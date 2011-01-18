#include "main.h"

#ifdef DEBUG
  #define CONF_DIR "conf/"
#else
  #define CONF_DIR "path/to/usr/dir/"
#endif

#define MAX_CHARS 100

int conf_read(char *filename, char *section, char *key, char **returned);
int conf_update(char *filename, char *section, char *key);
int conf_is_section(char line[MAX_CHARS]);
