#include <avr/io.h>
#include <util/delay.h>

int main()
{
  DDRD |= 1 << PIND0;

  while(1)
  {
    PORTD |= (1 << PIND0);
    _delay_ms(100);
    PORTD &= ~(1 << PIND0);
    _delay_ms(100);

    PORTD |= (1 << PIND1);
    _delay_ms(100);
    PORTD &= ~(1 << PIND1);
    _delay_ms(100);

    PORTD |= (1 << PIND2);
    _delay_ms(100);
    PORTD &= ~(1 << PIND2);
    _delay_ms(100);

    PORTD |= (1 << PIND3);
    _delay_ms(100);
    PORTD &= ~(1 << PIND3);
    _delay_ms(100);

    /*PORTD |= (1 << PIND4);
    _delay_ms(100);
    PORTD &= ~(1 << PIND4);
    _delay_ms(100);*/


  }
}


