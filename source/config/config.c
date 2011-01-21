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

int conf_reads(json_t *root, char *section, char *key, char **value) {
  json_t *section_obj = NULL, *value_obj = NULL;
  
  if(!json_is_object(root)) return 1;
  
  section_obj = json_object_get(root, section);
  if(!json_is_object(section_obj)) return 1;

  value_obj = json_object_get(section_obj, key);
  if(!json_is_string(value_obj)) return 1;
  
  *value = malloc(CONF_MAX * sizeof(char));
  strcpy(*value, json_string_value(value_obj));

  if(value == NULL) return 1;

  return 0;  
}

int conf_readi(json_t *root, char *section, char *key, int *value) {
  json_t *section_obj = NULL, *value_obj = NULL;
  
  if(!json_is_object(root)) return 1;
  
  section_obj = json_object_get(root, section);
  if(!json_is_object(section_obj)) return 1;

  value_obj = json_object_get(section_obj, key);
  if(!json_is_integer(value_obj)) return 1;
  
  if(json_typeof(value_obj) != JSON_INTEGER) return 1;
  
  *value = json_integer_value(value_obj);

  if(value == NULL) return 1;

  return 0;  
}

int conf_update(char *filename, char *section, char *key, char *value) {
  int   in_section = 0, updated = 0;
  FILE  *conf_file, *temp_file;
  char  path[CONF_MAX] = CONF_DIR, section_heading[CONF_MAX], line[CONF_MAX], 
        file_key[CONF_MAX];
  
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

int conf_is_section(char line[CONF_MAX]) {
  if(line[0] == '[') {
    return 1;
  }
  
  return 0;
}
