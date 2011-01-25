#include <string.h>
#include <jansson.h>
#include "utility.h"
#include "config.h"

cf_json *cf_read(const char *input) {
  json_t *root;
  json_error_t error;
  
  /* Ensure input is not NULL */ 
  if(!input) return NULL;
  
  /* Check if input contains JSON or is a path to a file containing JSON */
  if(strstr(input, ".json")) {
    root = json_load_file(input, 0, &error);
  }
  else {
    root = json_loads(input, 0, &error);
  }
  
  /* Prints out any errors encountered whilst parsing the JSON. */
  if(!root) {
    fprintf(stderr, "Error on line: %i, col: %i\n%s\n", error.line, 
            error.column, error.text);
    return NULL;
  }
  
  return root;
}

int cf_write(const cf_json *root, const char *input) {
  /* input is automatically formatted to ensure configuration files are 
     consistently formatted */
  if(json_dump_file(root, input, JSON_INDENT(2) || JSON_SORT_KEYS)) {
    return ERR_UNKNOWN;
  }
  
  return ERR_NONE;
}

void cf_free(cf_json *root) {
  /* jansson uses resource counting to keep track of resources. Each reference
     must be decremented in order to free it. */
  json_decref(root);
}

cf_json *cf_get_object(const cf_json *root, const char *key) {
  /* This returns a reference to the object stored in root. Therefore, if root
     is freed, this object will be destroyed as well */
  return json_object_get(root, key);
}

cf_json *cf_get_array(const cf_json *root, int key) {
  return json_array_get(root, key);
}

/* The following functions first get an object containing the required value
   and then convert the object into the required type */
const char *cf_get_string(const cf_json *root, const char *key) {
  cf_json *object = NULL;
  
  object = cf_get_object(root, key);
  if(!object) return NULL;
  
  return json_string_value(object);
}

int cf_get_integer(const cf_json *root, const char *key) {
  json_t *object = NULL;
  
  object = cf_get_object(root, key);
  if(!object) return 0;
  
  return json_integer_value(object);
}

double cf_get_double(const cf_json *root, const char *key) {
  json_t *object = NULL;
  
  object = cf_get_object(root, key);
  if(!object) return 0.0;
  
  return json_real_value(object);
}

int cf_set_object(cf_json *root, const char *key, cf_json *value) {
  /* Even if an object is created in this process, it is enough to call 
     cf_free on root due to jansson's resource counting */
  return json_object_set_new(root, key, value);
}

/* The following functions first convert value from the given type into a JSON
   object and then set that object onto the root object. */
int cf_set_string(cf_json *root, const char *key, const char *value) {
  json_t *object = NULL;

  object = json_string(value);
  if(object == NULL) return ERR_UNKNOWN;
  
  if(cf_set_object(root, key, object)) return ERR_UNKNOWN;
    
  return ERR_NONE;
}

int cf_set_integer(cf_json *root, const char *key, int value) {
  json_t *object = NULL;

  object = json_integer(value);
  if(object == NULL) return ERR_UNKNOWN;
  
  if(cf_set_object(root, key, object)) return ERR_UNKNOWN;
  
  return ERR_NONE;
}

int cf_set_double(cf_json *root, const char *key, double value) {
  json_t *object = NULL;

  object = json_real(value);
  if(object == NULL) return ERR_UNKNOWN;
  
  if(cf_set_object(root, key, object)) return ERR_UNKNOWN;
    
  return ERR_NONE;
}

/* This is handy for quickly checking whether a JSON object contains the 
   correct information at a particular point in the program. It is formatted so 
   that it will look identical to the configuration files */
int cf_printf(const cf_json *root) {
  if(json_dumpf(root, stdout, JSON_INDENT(2) || JSON_SORT_KEYS)) {
    return ERR_UNKNOWN;
  }
  
  return ERR_NONE;
}
