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

typedef enum ut_os_type {
  UT_OS_LINUX,
  UT_OS_OSX,
  UT_OS_WINDOWS
} ut_OSType;

typedef enum ut_error_code {
  UT_ERR_UNKNOWN = -1,	//unknown error
  UT_ERR_NONE = 0,		//no error
  UT_ERR_CLOSE,		//shutdown signal was recieved
  UT_ERR_BAD_PATH,		//file path is invalid
  UT_ERR_BAD_FILE,		//file is corrupt
  UT_ERR_BAD_ARG,		//invalid argument to a function
  UT_ERR_BAD_MODE,		//invalid run mode selected
  UT_ERR_BAD_ACTION,	//an invalid action was requested
  UT_ERR_EMPTY,			//no data to read
  UT_ERR_JSON_ENCODE,  //failed to encode data into JSON
  UT_ERR_JSON_DECODE,  //failed to decode JSON data
  UT_ERR_CURL_SETUP, //failed to setup CURL
  UT_ERR_OAUTH_SIGN, //failed to sign an oauth url
  UT_ERR_INVALID_RESPONSE, //response is corrupt
} ut_error_code;

/* test to see if a path is valid and writable */
int ut_test_path(const char *path);

int ut_isint(char *line);

#endif
