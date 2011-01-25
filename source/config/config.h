/**
 * @file   config.h
 * @author Rupert Madden-Abbott
 *
 * This module wraps the jansson library to simplify and standardise 
 * interaction with JSON objects. It provides functions for parsing JSON 
 * contained in either cfiguration files or network responses.
 */

#ifndef CONFIG_H
#define CONFIG_H

/* Represents a JSON structure (Renamed to maintain a consistent public API) */
typedef json_t cf_json;

/**
 * Allocate and return a JSON structure from either a file or a string. This
 * structure may then be traversed using the cf_read* functions in order to
 * get data.
 *
 * @warning Every structure created in this way must be cleaned up by calling  
 * cf_free when the structure is no longer needed.
 * 
 * Prints to stderr if an error in the JSON occured.
 *
 * @param  input A path to a json file or a JSON formatted string
 * @return NULL on error or the JSON object parsed from @p input
 */
cf_json *cf_read(const char *input);

/**
 * Writes a JSON object into the file specified by @p path. If @p path already
 * exists, the contents are overwritten. If @p path doesn't exist, it will be
 * created.
 * 
 * @param  root The JSON object to be written to file
 * @param  path The path of the file to be written to
 * @return err_code
 */
int cf_write(const cf_json *root, const char *path);

/**
 * Performs cleanup when a JSON object is finished with.
 * 
 * @param  root The JSON object to be destroyed
 * @return void
 */
void cf_free(cf_json *root);

///@{
/**
 * Each function returns a value (of the relevant type) stored inside the JSON 
 * object @p root based on its @p key. In the case of arrays, @p key is the 
 * index of the value.
 *
 * The object returned by cf_get_object is a reference to the actual value
 * stored in @p root. Therefore, if cf_free is called on @p root, this object 
 * will also be destroyed.
 *
 * @param  root A JSON object
 * @param  key  The name or index of the @p value to return
 * @return NULL, 0 or 0.0 on error or the value named @p key
 */
cf_json *cf_get_object(const cf_json *root, const char *key);
cf_json *cf_get_array(const cf_json *root, int key);
const char *cf_get_string(const cf_json *root, const char *key);
int cf_get_integer(const cf_json *root, const char *key);
double cf_get_double(const cf_json *root, const char *key);
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
int cf_set_object(cf_json *root, const char *key, cf_json *value);
int cf_set_string(cf_json *root, const char *key, const char *value);
int cf_set_integer(cf_json *root, const char *key, int value);
int cf_set_double(cf_json *root, const char *key, double value);
///@}

/**
 * Prints a clearly formatted representation of a JSON object. Intended for 
 * testing.
 *
 * @param  root A JSON object
 * @return error_code
 */
int cf_printf(const cf_json *root);

#endif
