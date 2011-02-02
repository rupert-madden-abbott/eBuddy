#ifndef GESTURE_INTERFACE_H
#define GESTURE_INTERFACE_H

#define GSI_NULL 2
#define GSI_PHER 1
#define GSI_OK 0

extern int gsi_happy_level1(void);
extern int gsi_fun_level1(void);
extern int gsi_notification(void);
extern int gsi_test(void);
extern int gsi_shake_head(void);
extern int gsi_move_arms(void);
extern int gsi_raise_arms(void);
extern int gsi_eyeflash(void);
int gsi_wave_right(void);
int gsi_wave_left(void);

extern int gsi_printLCD(const char* str);

/* representation of a gesture [needs to go in gesture] */
typedef struct gsi_reaction {
  int (*gesture)(void);	/* pointer to the gesture that should be called */
  const char *message;	/* string to display on the lcd screen */
} gsi_Reaction;

int gsi_react(const gsi_Reaction *resp);
#endif
