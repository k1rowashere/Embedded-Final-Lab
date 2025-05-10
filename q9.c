#include <avr/interrupt.h>
#include <avr/io.h>

uint8_t counter = 0;
uint8_t print_units = 0;

// increment counter every 1s
ISR(TIMER1_COMPA_vect) {
  counter++;
  counter %= 100;
}

// update display (every 5ms)
ISR(TIMER0_COMP_vect) {
  if (print_units) {
    PORTA &= ~(1 << PA0);
    PORTB = counter % 10;
    PORTA |= (1 << PA1);
  } else {
    PORTA &= ~(1 << PA1);
    PORTB = counter / 10;
    PORTA |= (1 << PA0);
  }
  print_units = !print_units;
}

int main() {
  DDRB = 0xFF; // Set PORTB as output
  DDRA = 0x03; // Set PA0 and PA1 as output

  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (0b101 << CS10); // CTC mode, prescaler 1024
  OCR1A = 10800;                           // 2s

  TCCR0 = (1 << WGM01) | (0b100 << CS00); // CTC mode, prescaler 64
  OCR0 = 216;                             // 5ms

  // Enable Timer1, Timer0 compare interrupt
  TIMSK = (1 << OCIE1A) | (1 << OCIE0);

  sei(); // Enable global interrupts

  while (1)
    ;
}