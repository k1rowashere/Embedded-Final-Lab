#include <avr/io.h>

int main() {
  DDRB = 0xFF;

  ADMUX = (1 << ADLAR) | (1 << REFS0) | (0b0100 << MUX0);
  ADCSRA = (1 << ADEN);

  while (true) {
    ADCSRA |= (1 << ADSC); // start conversion
    while (ADCSRA & (1 << ADSC))
      ; // wait for conversion to complete
    PORTB = ADCH;
  }
}