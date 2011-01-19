#include "config.h"

int conf_read(char *filename, char *section, char *key, char **returned) {
  int  in_section = 0;
  FILE *conf_file;
  char line[MAX_CHARS], section_heading[MAX_CHARS], file_key[MAX_CHARS],
       path[MAX_CHARS] = CONF_DIR;
  char *value = (char *)malloc(MAX_CHARS*sizeof(char));
  
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
        if(!strcmp(key, file_key)) {
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
  return 1;
}

int conf_update(char *filename, char *section, char *key, char *value) {
  int   in_section = 0, updated = 0;
  FILE  *conf_file, *temp_file;
  char  path[MAX_CHARS] = CONF_DIR, section_heading[MAX_CHARS], line[MAX_CHARS], 
        file_key[MAX_CHARS];
  
  sprintf(section_heading, "[%s]", section);
  strcat(path, filename);

  conf_file = fopen(path, "r+");
  if(!conf_file) {
    return 1;
  }
  
  temp_file = tmpfile();
  
  while(fgets(line, sizeof(line), conf_file)) {
    if(in_section) {  
      if(conf_is_section(line)) {
        fputs(line, temp_file);
        in_section = 0;
      }
      else {
        sscanf(line, "%s ", file_key);
        if(strcmp(key, file_key)) {
          fputs(line, temp_file);
        }
        else {
          updated = 1;
          fprintf(temp_file, "%s %s\n", file_key, value);
        }
      }
    }
    else if(conf_is_section(line) && strcmp(line, section_heading)) {
      fputs(line, temp_file);
      in_section = 1;
    }
    else {
      fputs(line, temp_file);
    }
  }
  if(updated) {
    rewind(temp_file);
    
    freopen(path, "w+", conf_file);
    if(!conf_file) {
      return 1;
    }  
    
    while(fgets(line, sizeof(line), temp_file)) {
      fputs(line, conf_file);
    }
  }
  
  fclose(conf_file);
  fclose(temp_file);
  
  if(!updated) {
    return 1;
  }
  return 0;
}

int conf_is_section(char line[MAX_CHARS]) {
  if(line[0] == '[') {
    return 1;
  }
  
  return 0;
}
