/**
 * @file   config.h
 * @author Rupert Madden-Abbott
 *
 * This module wraps the jansson library to simplify and standardise 
 * interaction with JSON objects. It provides functions for parsing JSON 
 * contained in either configuration files or network responses.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "main.h"

/* Represents a JSON structure (Renamed to maintain a consistent public API) */
typedef json_t conf;

/**
 * Allocate and return a JSON structure from either a file or a string. This
 * structure may then be traversed using the conf_read* functions in order to
 * get data.
 *
 * @warning Every structure created in this way must be cleaned up by calling  
 * conf_free when the structure is no longer needed.
 * 
 * Prints to stderr if an error in the JSON occured.
 *
 * @param  input A path to a json file or a JSON formatted string
 * @return NULL on error or the JSON object parsed from @p input
 */
conf *conf_read(const char *input);

/**
 * Writes a JSON object into the file specified by @p path. If @p path already
 * exists, the contents are overwritten. If @p path doesn't exist, it will be
 * created.
 * 
 * @param  root The JSON object to be written to file
 * @param  path The path of the file to be written to
 * @return err_code
 */
int conf_write(const conf *root, const char *path);

/**
 * Performs cleanup when a JSON object is finished with.
 * 
 * @param  root The JSON object to be destroyed
 * @return void
 */
void conf_free(conf *root);

///@{
/**
 * Each function returns a value (of the relevant type) stored inside the JSON 
 * object @p root based on its @p key.
 *
 * The object returned by conf_get_object is a reference to the actual value
 * stored in @p root. Therefore, if conf_free is called on @p root, this object 
 * will also be destroyed.
 *
 * @param  root A JSON object
 * @param  key  The name of the @p value to return
 * @return NULL, 0 or 0.0 on error or the value named @p key
 */
conf *conf_get_object(const conf *root, const char *key);
const char *conf_get_string(const conf *root, const char *key);
int conf_get_integer(const conf *root, const char *key);
double conf_get_double(const conf *root, const char *key);
///@}

///@{
/**
 * Each function sets a value, named @p key,  in @p root of the relevant type. 
 * 
 * If there is already a value named @p key, that value is overwritten.
 *
 * It does not matter if the old and new values differ in type e.g. A string 
 * could be set on a key previously of type int
 *
 * @param  root  A JSON object
 * @param  key   The name of the new @p value
 * @param  value The value to be stored in @p root
 * @return err_code
 */
int conf_set_object(conf *root, const char *key, conf *value);
int conf_set_string(conf *root, const char *key, const char *value);
int conf_set_integer(conf *root, const char *key, int value);
int conf_set_double(conf *root, const char *key, double value);
///@}

/**
 * Prints a clearly formatted representation of a JSON object. Intended for 
 * testing.
 *
 * @param  root A JSON object
 * @return error_code
 */
int conf_printf(const conf *root);

#endif
