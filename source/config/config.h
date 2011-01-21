#ifndef CONFIG_H
#define CONFIG_H

#include "main.h"

#ifdef DEBUG
  #define CONF_DIR "conf/"
#else
  #define CONF_DIR "path/to/usr/dir/"
#endif

#define MAX_CHARS 100

int conf_load(char *input, json_t **root);
int conf_printf(json_t *root);
int conf_read2(json_t *root, char *section, char *key, const char **value);

int conf_read(char *filename, char *section, char *key, char **returned);
int conf_update(char *filename, char *section, char *key, char *value);
int conf_is_section(char line[MAX_CHARS]);

#endif
