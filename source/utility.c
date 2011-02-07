#include <string.h>
#include <ctype.h>
#include "utility.h"


//test to see if a path is valid and writable
int ut_test_path(const char *path) {
  FILE *file;
	
  //try to open the file for writing
  file = fopen(path, "w");
  
  if(!file) {
    return 0;
  }
  
  fclose(file);
  return 1;
}

int ut_isint(char *line) {
  unsigned int i;
  int rc;
  
  //reject empty strings
  rc = strlen(line);
  if(rc == 0) {
    return UT_ERR_UNKNOWN;
  }
  
  for(i = 0; i < strlen(line); i++) {
    //reject characters that aren't digits
    rc = isdigit(line[i]);
    if(!rc) {
      return UT_ERR_UNKNOWN;
    }
  }
  
  return UT_ERR_NONE;  
}

