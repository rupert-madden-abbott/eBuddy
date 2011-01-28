#include <stdio.h>
#include "utility.h"

/* test to see if a path is valid and writable */
int test_path(const char *path) {
  FILE *file;
	
  file = fopen(path, "w");
  
  if(!file) {
    return 0;
  }
  
  fclose(file);
  return 1;
}
