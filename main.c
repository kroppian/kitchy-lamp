#include <avr/io.h>
#include <time.h>
#include <avr/interrupt.h>  

/* IO macros */
#define LED1 PIND0
#define LED2 PIND1
#define LED3 PIND2
#define LED4 PIND3
#define LED5 PIND4

#define FLOOR_SWITCH PINB0
#define ROT_A PINB1
#define ROT_B PINB2
#define ROT_BTN PINB3

#define MANUAL_SWITCH PINB4

#define DIMMER PINB1

/* Timing */
#define RAW_TIMEOUT ((((timeout_in_secs * FREQ) / 8) / OCR2A))
// TODO this seems wrong
#define FREQ 500000

/* Helpful */ 
#define ON 1
#define OFF 0

/* Functions */
void check_mat(void);
void check_timeout(void);

int light;
int timeout_in_secs;
int counter;
int manual_mode;

int main()
{

  /* Setting up IO */
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

  /* Setting up PWM */

  // Initialize
  TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1<<COM1A0);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10 );
  ICR1 =  19999;

  //int offset = 18000;
  int offset = 500;

  OCR1A = ICR1 - offset;

  /* Set up timing*/
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

  /* Setting up state */
  light = 0;
  timeout_in_secs = 3;
  counter = 0; 
  manual_mode = bit_is_clear(PINB, MANUAL_SWITCH);


  /* Time to chase tails */ 
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

void check_timeout()
{

  if(! bit_is_clear(PINB, FLOOR_SWITCH) && counter >= RAW_TIMEOUT){
    light = OFF; 
  }    

}

ISR(TIMER2_COMPA_vect)
{

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
  } else {
    PORTD &= ~((1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED4)  | (1 << LED5));
    counter = 0;
  }
  

}


