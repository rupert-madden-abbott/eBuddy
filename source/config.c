#include "config.h"

cf_Json *cf_read(const char *input) {
  json_t *root;
  
  //Ensure input is set 
  if(!input) return NULL;
  
  //Check if input contains JSON or is a path to a file containing JSON
  if(strstr(input, ".json")) {
    root = json_load_file(input, 0, NULL);
  }
  else {
    root = json_loads(input, 0, NULL);
  }
  
  //Return NULL if the JSON could not be decoded
  if(!root) return NULL;
  
  return root;
}

int cf_write(const cf_Json *root, const char *path) {
  //Input is automatically formatted to provide consistent config files
  if(json_dump_file(root, path, JSON_INDENT(2) || JSON_SORT_KEYS)) {
    return UT_ERR_JSON_ENCODE;
  }
  
  return UT_ERR_NONE;
}

cf_Json *cf_create(const char *str, const char *path) {
  int rc;
  json_t *root;
  json_error_t error;

  //Ensure str and path are set
  if(!str || !path) {
    return NULL;
  }
  
  //Encode the string into a JSON object
  root = json_loads(str, 0, &error);
  if(!root) {
    return NULL;
  }
  
  //Write the JSON object to a file
  rc = cf_write(root, path);
  if(rc) {
    cf_free(root);
    return NULL;
  }
  
  return root;
}

void cf_free(cf_Json *root) {
  /* Jansson uses resource counting to keep track of resources. Each reference
     must be decremented in order to free it. */
  json_decref(root);
}

cf_Json *cf_get_object(const cf_Json *root, const char *key) {
  /* This returns a reference to the object stored in root. Therefore, if root
     is freed, this object will be destroyed as well */
  return json_object_get(root, key);
}

cf_Json *cf_get_array(const cf_Json *root, int key) {
  return json_array_get(root, key);
}

/* The following functions first get an object containing the required value
   and then convert the object into the required type */
char *cf_get_string(const cf_Json *root, const char *key) {
  cf_Json *object = NULL;
  
  object = cf_get_object(root, key);
  if(!object) return NULL;
  
  return (char *)json_string_value(object);
}

int cf_get_integer(const cf_Json *root, const char *key) {
  json_t *object = NULL;
  
  object = cf_get_object(root, key);
  if(!object) return 0;
  
  return json_integer_value(object);
}

double cf_get_double(const cf_Json *root, const char *key) {
  json_t *object = NULL;
  
  object = cf_get_object(root, key);
  if(!object) return 0.0;
  
  return json_real_value(object);
}

char *cf_get_nstring(const cf_Json *root, const char *key, int buffer) {
  int rc;
  char *check;
  
  check = cf_get_string(root, key);
  if(!check) {
    return NULL;
  }
  
  rc = strlen(check);
  if(rc > buffer) {
    return NULL;
  }
  
  return check;  
}

int cf_set_object(cf_Json *root, const char *key, cf_Json *value) {
  /* Even if an object is created in this process, it is enough to call 
     cf_free on root due to jansson's resource counting */
  return json_object_set_new(root, key, value);
}

/* The following functions first convert value from the given type into a JSON
   object and then set that object onto the root object. */
int cf_set_string(cf_Json *root, const char *key, const char *value) {
  json_t *object = NULL;

  object = json_string(value);
  if(object == NULL) return UT_ERR_JSON_ENCODE;
  
  if(cf_set_object(root, key, object)) return UT_ERR_JSON_ENCODE;
    
  return UT_ERR_NONE;
}

int cf_set_integer(cf_Json *root, const char *key, int value) {
  json_t *object = NULL;

  object = json_integer(value);
  if(object == NULL) return UT_ERR_JSON_ENCODE;
  
  if(cf_set_object(root, key, object)) return UT_ERR_JSON_ENCODE;
  
  return UT_ERR_NONE;
}

int cf_set_double(cf_Json *root, const char *key, double value) {
  json_t *object = NULL;

  object = json_real(value);
  if(object == NULL) return UT_ERR_JSON_ENCODE;
  
  if(cf_set_object(root, key, object)) return UT_ERR_JSON_ENCODE;
    
  return UT_ERR_NONE;
}

/* This is handy for quickly checking whether a JSON object contains the 
   correct information at a particular point in the program. It is formatted so 
   that it will look identical to the configuration files */
int cf_printf(const cf_Json *root) {
  if(json_dumpf(root, stdout, JSON_INDENT(2) || JSON_SORT_KEYS)) {
    return UT_ERR_JSON_ENCODE;
  }
  
  return UT_ERR_NONE;
}
