#include <avr/interrupt.h>
#include <avr/io.h>

const uint32_t presacler = 1024;
const uint32_t ticks_per_second = F_CPU / prescaler;

uint32_t counter = 0;

// toggle led (every 500ms)
ISR(TIMER0_OVF_vect) {
  counter++;
  if (counter % (ticks_per_second / 2) == 0) { // every 500ms
    PORTB ^= (1 << PB3); // Toggle LED on PB3
    counter = 0;
  }
}

int main() {
  DDRB = (1 << PB3); // Set PB3 as output

  TCCR0 = (0b101 << CS00); // Normal mode, prescaler 1024

  // Enable Timer0 compare interrupt
  TIMSK = (1 << TOIE0);

  sei(); // Enable global interrupts

  while (1)
    ;
}