#include "config.h"

int conf_read(char *filename, char *section, char *key, char **returned) {
  int   in_section = 0;
  FILE  *conf_file;
  char  line[MAX_CHARS], section_heading[MAX_CHARS], file_key[MAX_CHARS],
        path[MAX_CHARS] = CONF_DIR;
  char  *value = (char *)malloc(MAX_CHARS*sizeof(char));
  
  sprintf(section_heading, "[%s]", section);
  strcat(path, filename);
  
  conf_file = fopen(path, "r");
  if(!conf_file) {
    return 1;
  }

  while(fgets(line, MAX_CHARS, conf_file)) {
    if(in_section) {
      if(conf_is_section(line)) {
        in_section = 0;
      }
      else {
        sscanf(line, "%s %[^\t\n]", file_key, value);
        if(strcmp(key, file_key) == 0) {
          fclose(conf_file);
          *returned = value;
          return 0;
        }
      }
    }
    else if(conf_is_section(line) && strcmp(line, section_heading)) {
      in_section = 1;
    }
  }
  return -1;
}

int conf_update(char* filename, char* section, char* key) {
  //FILE* conf_file;
  
  //conf_file = fopen(CONF_DIR "/" filename, "w");
  
  return 0;
}

int conf_is_section(char line[MAX_CHARS]) {
  if(line[0] == '[') {
    return 1;
  }
  
  return 0;
}
