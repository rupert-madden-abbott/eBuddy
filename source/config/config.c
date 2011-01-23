#include "config.h"

int conf_load(char *input, json_t **root) {
  json_error_t error;
  
  if(!input) return 1;
  
  if(strstr(input, ".json")) {
    *root = json_load_file(input, &error);
  }
  else {
    *root = json_loads(input, &error);
  }
  
  if(!*root) {
    fprintf(stderr, "Error on line: %i: %s", error.line, error.text);
    return 1;
  }
  
  return 0;
}

int conf_printf(json_t *root) {
  if(json_dumpf(root, stdout, JSON_INDENT(2))) return 1;
  
  return 0;
}

int conf_reado(json_t *root, char *key, json_t **object) {
  if(!json_is_object(root)) return 1;

  *object = json_object_get(root, key);
  if(*object == NULL) return 1;

  return 0;
}

int conf_reads(json_t *root, char *key, char *value) {
  json_t *object = NULL;
  
  if(conf_reado(root, key, &object)) return 1;
  if(!json_is_string(object)) return 1;

  strcpy(value, json_string_value(object));

  if(value == NULL) return 1;

  return 0;  
}

int conf_readi(json_t *root, char *key, int *value) {
  json_t *object = NULL;

  if(conf_reado(root, key, &object)) return 1;
  if(!json_is_integer(object)) return 1;  

  *value = json_integer_value(object);
  if(value == NULL) return 1;

  return 0;  
}

int conf_updates(json_t **root, char *key, char *value) {
  json_t *object = NULL;

  if(!json_is_object(*root)) return 1;

  object = json_string(value);
  if(object == NULL) return 1;
  
  if(json_object_set(*root, key, object)) return 1;
  
  return 0;
}

int conf_updatei(json_t **root, char *key, int value) {
  json_t *object = NULL;

  if(!json_is_object(*root)) return 1;

  object = json_integer(value);
  if(object == NULL) return 1;
  
  if(json_object_set(*root, key, object)) return 1;
  
  return 0;
}

int conf_write(char *input, json_t *root) {
  if(json_dump_file(root, input, JSON_INDENT(2))) return 1;
  
  return 0;
}
