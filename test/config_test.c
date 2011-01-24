#include "config.h"
#include "test.h"

int main(void) {
  int i, rc;
  double d;
  const char *json_string = "[{\"object\":{\"string\":\"a string\"},\"int\":5,\""
                            "double\":9.4}]", *s, *s2;
  conf *root, *file_root, *array, *object;

  array = conf_read(json_string);
  if(!array) fail("Failed to read JSON string");

  root = conf_get_array(array, 0);
  if(!root) fail("Failed to get array index");
  
  object = conf_get_object(root, "object");
  if(!object) fail("Failed to get object");
  
  s = conf_get_string(object, "string");
  if(!s) fail("Failed to get string");
  if(strcmp(s, "a string")) fail("Returned incorrect string");
  
  i = conf_get_integer(root, "int");
  if(i == 0) fail("Failed to get integer");
  if(i != 5) fail("Returned incorrect integer");
    
  d = conf_get_double(root, "double");
  if(d == 0.0) fail("Failed to get double");
  if(d != 9.4) fail("Returned incorrect double");
  
  rc = conf_set_object(root, "new object", object);
  if(rc || !conf_get_object(root, "new object")) fail("Failed to set object");
  if(!conf_get_string(conf_get_object(root, "new object"), 
     "string")) fail("Object set to incorrect value");

  rc = conf_set_string(root, "set string", "test string");
  s2 = conf_get_string(root, "set string"); 
  if(rc || !s2) fail("Failed to set string");
  if(strcmp(s2, "test string")) fail("String set to incorrect value");
  
  rc = conf_set_integer(root, "set integer", 20);
  i = conf_get_integer(root, "set integer"); 
  if(rc || i == 0) fail("Failed to set integer");
  if(i != 20) fail("Integer set to incorrect value");
  
  rc = conf_set_double(root, "set double", 7.4566);
  d = conf_get_double(root, "set double"); 
  if(rc || d == 0.0) fail("Failed to set double");
  if(d != 7.4566) fail("Double set to incorrect value");

  rc = conf_printf(root);
  if(rc) fail("Failed to print JSON object");
  
  conf_free(root);
  if(!conf_printf(root)) fail("Failed to derefence JSON object");
  
  file_root = conf_read("test/test.json");
  if(!file_root) fail("Failed to read JSON file");
  
  rc = conf_write(file_root, "test/test.json");
  if(rc) fail("Failed to write JSON object to file");
   
  return 0;
}
