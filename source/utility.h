#ifndef UTILITY_H
#define UTILITY_H

#ifdef __linux__
#define OS LINUX
#endif
#ifndef NeXTBSD
#ifdef __APPLE__
#define OS OSX
#endif
#endif

#define ONE_MIN (60)
#define ONE_HOUR (60*60)
#define ONE_DAY (24*60*60)
#define SECONDS_IN_AN_HOUR (60*60)

enum os {
  LINUX,
  OSX,
  WINDOWS
};

typedef enum error_code {
  ERR_UNKNOWN = -1,	/* unknown error */ 
  ERR_NONE = 0,		/* no error */
  ERR_BAD_PATH,		/* file path is invalid */
  ERR_BAD_FILE,		/* file is corrupt */
  ERR_BAD_ARG
} error_code;

/* test to see if a path is valid and writable */
int test_path(const char *path);

#endif
