#include <avr/io.h>
#include <time.h>
#include <avr/interrupt.h>  
#include "librot.h"

/* IO macros */
#define LED1 PIND0
#define LED2 PIND1
#define LED3 PIND2
#define LED4 PIND3
#define LED5 PIND4

#define FLOOR_SWITCH PINB0
#define ROT_A PINB3
#define ROT_B PINB4
#define ROT_BTN PINB5

#define MANUAL_SWITCH PINB2

#define DIMMER PINB1

/* Timing */
#define RAW_TIMEOUT ((((timeout_in_secs * FREQ) / 8) / OCR2A))
// TODO this seems wrong
#define FREQ 500000

/* Helpful */ 
#define ON 1
#define OFF 0

/* dimmer */ 
#define DIMMER_RESOLUTION 100

/* Functions */
void check_mat(void);
void check_timeout(void);
void update_dimmer(void);

int light;
int timeout_in_secs;
int counter;
int manual_mode;
int is_rot; 
int offset;

int main()
{

  /* ##################
   * # Setting up IO 
   * ################## */

  // turning on the output leds
  DDRD |= 1 << LED1;
  DDRD |= 1 << LED2;
  DDRD |= 1 << LED3;
  DDRD |= 1 << LED4;
  DDRD |= 1 << LED5;

  // turning the floor switch to input
  DDRB &= ~(1 << FLOOR_SWITCH);
  PORTB |= (1 << FLOOR_SWITCH);

  // turning the manual switch to input
  DDRB &= ~(1 << MANUAL_SWITCH);
  PORTB |= (1 << MANUAL_SWITCH);

  // turning the dimmer pwm output on 
  DDRB |= 1 << DIMMER;

  /* ##################
   * # Setting up PWM/dimming
   * ################## */

  // Initialize
  TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1<<COM1A0);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10 );
  ICR1 =  19999;

  offset = DIMMER_RESOLUTION;

  OCR1A = ICR1 - offset;

  /* ##################
   * # Set up timing 
   * ################## */
  sei();

  // prescaler of 8
  TCCR2B |= 1 << CS11;
  TCCR2B &=  ~(1 << CS10);
  TCCR2B &=  ~(1 << CS12);

  // CTC mode
  TCCR2B |= 1<<WGM12;

  // timer interupt mask
  TIMSK2 |= 1<< OCIE1A;

  OCR2A = 100;

  /* ##################
   * # Setting up state   
   * ################## */
  light = 0;
  timeout_in_secs = 3;
  counter = 0; 
  manual_mode = bit_is_clear(PINB, MANUAL_SWITCH);

  /* ##################
   * # Setting rotary encoder   
   * ################## */
  initialize_rot(PINSETB, ROT_A, PINSETB, ROT_B);
  is_rot = 0;

  /* ##################
   * # Time to chase tails   
   * ################## */
  while(1)
  {

  }

}

void check_mat(void)
{

  if(bit_is_clear(PINB, FLOOR_SWITCH)){
    light = ON; 
    counter = 0;
  }  

}

void check_timeout(void)
{

  if(! bit_is_clear(PINB, FLOOR_SWITCH) && counter >= RAW_TIMEOUT){
    light = OFF; 
  }    

}

void update_dimmer(void)
{

  if(! is_rot && rot_rotating() == RIGHT){
    offset += DIMMER_RESOLUTION;
    is_rot = 1;
  }else if (! is_rot && rot_rotating() == LEFT){
    offset -= DIMMER_RESOLUTION;
    is_rot = 1;
  }else if(rot_rotating() == NO_ROT){
    is_rot = 0; 
  }

  if(offset < 0){
    offset = 0; 
  }else if(offset > 19999){
    offset = 18000;
  }

  OCR1A = ICR1 - offset;

}

ISR(TIMER2_COMPA_vect)
{

  rot_poll();

  if(bit_is_clear(PINB, MANUAL_SWITCH)){
    light = ON;
    counter = 0;
    manual_mode = ON;
  // } else if(prog_mode){
    // timer_set;
  // }else{
  }else{
    // make sure to turn the light off if it's no longer on
    if(manual_mode){
      manual_mode = OFF;
      light = OFF;
    }
    check_mat();
    if(light) check_timeout();
  }

  if(light){
    PORTD |= (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED4) | (1 << LED5);
    if(counter < RAW_TIMEOUT){
      counter++;
    }
    update_dimmer();
  } else {
    PORTD &= ~((1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED4)  | (1 << LED5));
    counter = 0;
  }
  

}


