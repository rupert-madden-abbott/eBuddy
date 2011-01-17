#include "config.h"

char *conf_read(char *filename, char *section, char *key) {
  int   in_section = 0;
  FILE  *conf_file;
  char  line[MAX_CHARS], section_heading[MAX_CHARS] = "[", file_key[MAX_CHARS],
        path[MAX_CHARS] = CONF_DIR;
  char  *value = (char *)malloc(MAX_CHARS*sizeof(char));
  
  strcat(section_heading, section);
  strcat(section_heading, "]");
  
  strcat(path, filename);
  
  conf_file = fopen(path, "r");
  if(conf_file == NULL) {
    perror("Error");
    exit(1);
  }
  else {
    while(fgets(line, MAX_CHARS, conf_file)) {
      if(in_section) {
        if(conf_is_section(line)) {
          in_section = 0;
        }
        else {
          sscanf(line, "%s %[^\t\n]", file_key, value);
          if(strcmp(key, file_key) == 0) {
            fclose(conf_file);
            return value;
          }
        }
      }
      else if(conf_is_section(line) && strcmp(line, section_heading)) {
        in_section = 1;
      }
    }
  }
  return "";
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
