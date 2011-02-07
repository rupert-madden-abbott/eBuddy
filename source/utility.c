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


