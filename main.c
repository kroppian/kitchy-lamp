#include <avr/io.h>
#include <util/delay.h>
#include <time.h>
#include <avr/interrupt.h>  

#define LED1 PIND0
#define LED2 PIND1
#define LED3 PIND2
#define LED4 PIND3
#define LED5 PIND4

#define FLOOR_SWITCH PINB0

#define PAUSE_FACTOR 1

#define ON 1
#define OFF 0

#define FREQ 1000000

void check_mat(void);
void check_timeout(void);

int light;
int timeout_in_secs;
int counter;

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

  /* Set up timing*/
  sei();

  // prescaler of 8
  TCCR2B |= 1 << CS11 | 1<<WGM12;
  TCCR2B &=  ~(1 << CS10);
  // timer interupt mask
  TIMSK2 |= 1<< OCIE1A;

  OCR2A = 100;

  /* Setting up state */
  light = 0;
  timeout_in_secs = 3;

  counter = 0; 

  while(1)
  {

  }

}


void check_mat(void){

  if(bit_is_clear(PINB, FLOOR_SWITCH)){
    light = ON; 
  }  

}

void check_timeout(){

  if(counter >= ((((timeout_in_secs * FREQ) / 8) / 200))){
    light = OFF; 
  }    

}

ISR(TIMER2_COMPA_vect){

  // if(prog_mode){
    // timer_set;
  // }else{
    check_mat();
    if(light) check_timeout();
  // }

  if(light){
    PORTD |= (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED4) | (1 << LED5);

    counter++;
    if(counter >= ((((timeout_in_secs * FREQ) / 8) / 200))){
      counter = ((((timeout_in_secs * FREQ) / 8) / 200));
    }

  } else {
    PORTD &= ~((1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED4)  | (1 << LED5));
    counter = 0;
  }
  

}


