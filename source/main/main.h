#ifdef __linux__
#define OS "linux"
#endif
#ifndef NeXTBSD
#ifdef __APPLE__
#define OS "mac"
#endif
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG 1

void end(void);
int main_loop(void);
