#ifndef CONFIG_H
#define CONFIG_H

#include "main.h"

#ifdef DEBUG
  #define CONF_DIR "conf/"
#else
  #define CONF_DIR "path/to/usr/dir/"
#endif

int conf_load(char *input, json_t **root);
int conf_printf(json_t *root);
int conf_reado(json_t *root, char *key, json_t **value_obj);
int conf_reads(json_t *root, char *key, char *value);
int conf_readi(json_t *root, char *key, int *value);
int conf_updates(json_t **root, char *key, char *value);
int conf_updatei(json_t **root, char *key, int value);
int conf_write(char *input, json_t *root);

#endif
