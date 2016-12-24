#include <avr/io.h>
#include <util/delay.h>

#define LED1 PIND0
#define LED2 PIND1
#define LED3 PIND2
#define LED4 PIND3
#define LED5 PIND4

#define FLOOR_SWITCH PINB0

int main()
{

  // turning on the output leds
  DDRD |= 1 << LED1;
  DDRD |= 1 << LED2;
  DDRD |= 1 << LED3;
  DDRD |= 1 << LED4;
  DDRD |= 1 << LED5;

  // turning the floor switch to input
  DDRB &= ~(1 << FLOOR_SWITCH);

  PORTB |= (1 << FLOOR_SWITCH);

  while(1)
  {

    if(bit_is_clear(PINB, FLOOR_SWITCH)){
    
      PORTD |= (1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED4) | (1 << LED5);

    } else {
    
      PORTD &= ~((1 << LED1) | (1 << LED2) | (1 << LED3) | (1 << LED4) | (1 << LED4)  | (1 << LED5));

    }

  }

}


