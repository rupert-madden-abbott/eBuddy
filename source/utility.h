#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>

#ifdef __linux__
#define UT_OS UT_OS_LINUX
#endif
#ifndef NeXTBSD
#ifdef __APPLE__
#define UT_OS UT_OS_OSX
#endif
#endif

#define UT_MIN (60)
#define UT_HOUR (60*60)
#define UT_DAY (24*60*60)

#define UT_EPOCH_YEAR 1900

#define UT_MICRO 1000

/**
 * @enum ut_os_type
 *
 * Defines operating system types.
 *
 * Contains the following values:
 *
 * UT_OS_LINUX     - Linux
 * UT_OS_OSX       - Mac OSX
 * UT_OS_WINDOWS   - Windows
 */
typedef enum ut_os_type {
  UT_OS_LINUX,
  UT_OS_OSX,
  UT_OS_WINDOWS
} ut_OSType;

/**
 * @emum ut_error_code
 *
 * Defines shared error codes.
 *
 * Contains the following values:
 *
 * UT_ERR_UNKNOWN        - Unknown error
 * UT_ERR_NONE           - No error
 * UT_ERR_CLOSE          - Shutdown signal was recieved
 * UT_ERR_BAD_PATH       - File path is invalid
 * UT_ERR_BAD_FILE       - File is corrupt
 * UT_ERR_BAD_ARG        - Invalid argument to a function
 * UT_ERR_BAD_MODE       - Invalid run mode selected
 * UT_ERR_BAD_ACTION     - An invalid action was requested
 * UT_ERR_EMPTY          - No data to read
 * UT_ERR_JSON_ENCODE    - Failed to encode data into JSON
 * UT_ERR_JSON_DECODE    - Failed to decode JSON data
 * UT_ERR_CURL_SETUP     - Failed to setup CURL
 * UT_ERR_OAUTH_SIGN     - Failed to sign an oauth url
 * UT_ERR_INVALID_RESPONSE - Response is corrupt
 */
typedef enum ut_error_code {
  UT_ERR_UNKNOWN = -1,
  UT_ERR_NONE = 0,
  UT_ERR_CLOSE,
  UT_ERR_BAD_PATH,
  UT_ERR_BAD_FILE,
  UT_ERR_BAD_ARG,
  UT_ERR_BAD_MODE,
  UT_ERR_BAD_ACTION,
  UT_ERR_EMPTY,
  UT_ERR_JSON_ENCODE,
  UT_ERR_JSON_DECODE,
  UT_ERR_CURL_SETUP,
  UT_ERR_OAUTH_SIGN,
  UT_ERR_INVALID_RESPONSE,
} ut_ErrorCode;

/**
 * Tests to see if a path is valid and writable by attempting to create a file
 *
 * @param path The path to check
 * @return 1 on success, 0 on failure
 */
int ut_test_path(const char *path);

int ut_isint(char *line);

#endif
