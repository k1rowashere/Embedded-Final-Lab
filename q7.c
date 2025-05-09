#include <avr/interrupt.h>
#include <avr/io.h>

ISR(INT1_vect) {
  if (PINA & (1 << PA0)) // switch not pressed (active low)
    PORTB |= (1 << PB7); // buzzer on
}

int main() {
  DDRD = (0 << PD3);  // sensor INT1
  DDRA = (0 << PA0);  // switch
  PORTA = (1 << PA0); // pull-up

  DDRB = (1 << PB7); // buzzer

  MCUCR = (0b11 << ISC10); // INT1: rising edge
  GICR = (1 << INT1);
  sei();

  while (true) {
    if ((PINA & (1 << PA0)) == 0) // switch  pressed (active low)
      PORTB &= ~(1 << PB7);       // buzzer off
  }
}