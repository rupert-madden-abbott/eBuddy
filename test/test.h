#ifndef TEST_H
#define TEST_H

#include <stdlib.h>

#define fail(msg)                                                           \
        do {                                                                \
          fprintf(stderr, "%s[%i]: %s\n", __FILE__, __LINE__, msg);         \
          exit(1);                                                          \
        } while(1);                                     

#endif
