#ifndef CONFIG_H
#define CONFIG_H

#include "main.h"

#ifdef DEBUG
  #define CONF_DIR "conf/"
#else
  #define CONF_DIR "path/to/usr/dir/"
#endif

#define CONF_MAX 100

int conf_load(char *input, json_t **root);
int conf_printf(json_t *root);
int conf_reads(json_t *root, char *section, char *key, char **value);
int conf_readi(json_t *root, char *section, char *key, int *value);
int conf_reado(json_t *root, char *section, char *key, json_t **value_obj);

int conf_update(char *filename, char *section, char *key, char *value);
int conf_is_section(char line[CONF_MAX]);

#endif
