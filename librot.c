
#include "librot.h"
#include <avr/interrupt.h>  


// 0 for left 
// 1 for no rotation
// 2 for right
unsigned short state;

unsigned short pin_set_a;
unsigned short pin_set_b;

int pina;
int pinb;

void initialize_rot(int new_pin_seta, int new_pina, 
     int new_pin_setb, int new_pinb){

  pin_set_a = new_pin_seta;
  pin_set_b = new_pin_setb;

  state = 0;

  pina = new_pina;
  pinb = new_pinb;

  sei();

}

int get_rot_pins(int set){
  
  int set_state;

  if(set == PINSETB){
    set_state = PINB;
  }else if(set == PINSETC){
    set_state = PINC;
  }else if(set == PINSETD){
    set_state = PIND; 
  }else {
    set_state = -1; 
  }

  return set_state;
  
}

int rot_rotating(void){

  return state;

}

void rot_poll(void){

  int new_state_a =  get_rot_pins(pin_set_a);
  int new_state_b =  get_rot_pins(pin_set_b);

  int A = bit_is_clear(new_state_a,pina);
  int B = bit_is_clear(new_state_b,pinb);

  if ( ! A && ! B ){
    state = NO_ROT;
  } else if (state == NO_ROT && A){
    state = RIGHT;
  } else if (state == NO_ROT && B){
    state = LEFT; 
  }

}

