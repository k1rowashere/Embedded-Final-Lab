#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint32_t ticks;
ISR(TIMER1_OVF_vect) { ticks++; }
const double ticks_per_second = (uint32_t)F_CPU / (1ull << 16);

void delay(uint32_t n) {
  // delay n seconds using timer1 ovf
  ticks = 0;

  while (ticks < n * ticks_per_second)
    ; // F_CPU / (N * 2^16) = 21.093 (N = 1, 16-bit timer)
}

int main() {
  DDRB = (1 << PB0) | (1 << PB1);
  TCCR1B = (0b001 << CS10); // prescaler 1: for most accurate delay
  TIMSK = (1 << TOIE1);     // enable timer1 overflow interrupt
  sei();

  uint16_t counter = 0;
  while (1) {
    if (counter % 2 == 0) // T = 4 seconds
      PORTB ^= (1 << PB0);
    if (counter % 5 == 0)
      PORTB ^= (1 << PB1); // T = 10 seconds
    delay(1);
    counter++;
  }
}