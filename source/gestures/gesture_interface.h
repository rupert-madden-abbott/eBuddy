#include "gestures.h"

#define GSI_NULL 2
#define GSI_PHER 1
#define GSI_OK 0

extern int gsi_happy_level1(void);
extern int gsi_fun_level1(void);
extern int gsi_notification(void);
extern int gsi_test(void);
extern int gsi_printLCD(char* str);

/* representation of a gesture [needs to go in gesture] */
typedef struct gsi_Response {
  void (*gesture)(void);	/* pointer to the gesture that should be called */
  char *message;	/* string to display on the lcd screen */
} gsi_Response;

int gsi_respond(gsi_Response *resp);
