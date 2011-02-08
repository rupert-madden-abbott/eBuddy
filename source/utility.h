#ifndef UTILITY_H
#define UTILITY_H

/**
 * @define UT_OS
 *
 * Contains a ut_os_type representing the type of the
 * current operating system.
 */
#ifdef __linux__
#define UT_OS UT_OS_LINUX
#endif
#ifndef NeXTBSD
#ifdef __APPLE__
#define UT_OS UT_OS_OSX
#endif
#endif

/**
 * @define UT_MIN
 *
 * The number of seconds in a minute
 */
#define UT_MIN (60)

/**
 * @define UT_HOUR
 *
 * The number of seconds in an hour
 */
#define UT_HOUR (60*60)

/**
 * @define UT_DAY
 *
 * The number of seconds in a day
 */
#define UT_DAY (24*60*60)

/**
 * @define UT_EPOCH_YEAR
 *
 * The year of the unix epoch
 */
#define UT_EPOCH_YEAR 1900

/**
 * @define UT_MICRO
 *
 * The number of micro seconds in a second
 */
#define UT_MICRO 1000

/**
 * @enum ut_os_type
 *
 * Defines operating system types.
 *
 * @member UT_OS_LINUX     - Linux
 * @member UT_OS_OSX       - Mac OSX
 * @member UT_OS_WINDOWS   - Windows
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
 * @member UT_ERR_UNKNOWN          - Unknown error
 * @member UT_ERR_NONE             - No error
 * @member UT_ERR_CLOSE            - Shutdown signal was recieved
 * @member UT_ERR_BAD_PATH         - File path is invalid
 * @member UT_ERR_BAD_FILE         - File is corrupt
 * @member UT_ERR_BAD_ARG          - Invalid argument to a function
 * @member UT_ERR_BAD_MODE         - Invalid run mode selected
 * @member UT_ERR_BAD_ACTION       - An invalid action was requested
 * @member UT_ERR_EMPTY            - No data to read
 * @member UT_ERR_JSON_ENCODE      - Failed to encode data into JSON
 * @member UT_ERR_JSON_DECODE      - Failed to decode JSON data
 * @member UT_ERR_CURL_SETUP       - Failed to setup CURL
 * @member UT_ERR_OAUTH_SIGN       - Failed to sign an oauth url
 * @member UT_ERR_INVALID_RESPONSE - Network response is corrupt
 * @member UT_ERR_MUTEX_LOCK       - Failed to lock a mutex
 * @member UT_ERR_MUTEX_UNLOCK     - Failed to unlock a mutex
 * @member UT_ERR_MEMORY           - No memory for resource allocation
 * @member UT_ERR_POINTER_NULL     - Attempt to access pointer set to NULL
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
  UT_ERR_MUTEX_LOCK,
  UT_ERR_MUTEX_UNLOCK,
  UT_ERR_MEMORY,
  UT_ERR_POINTER_NULL
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
