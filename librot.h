#ifndef librot
#define librot

#include <avr/io.h>

#define LEFT 0
#define NO_ROT 1
#define RIGHT 2

#define PINSETB 0
#define PINSETC 1
#define PINSETD 2

void initialize_rot(int new_pin_seta, int new_pina, 
     int new_pin_setb, int new_pinb);

int rot_rotating(void);

void rot_poll(void);

int get_rot_pins(int set);

#endif

