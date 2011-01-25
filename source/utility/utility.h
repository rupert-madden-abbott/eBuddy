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

enum os { LINUX, OSX, WINDOWS };

typedef enum error_code {
  err_unknown = -1,	/* unknow error */
  err_none = 0,		/* no error */
  err_bad_path,		/* file path is invalid */
  err_bad_file		/* file is corrupt */
} error_code;

#endif
