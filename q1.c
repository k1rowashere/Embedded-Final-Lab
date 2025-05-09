#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

void serial_print(const char *str);
void serial_init(uint32_t baud);

double ticks_per_second = (float)F_CPU / 64;
volatile uint16_t ticks;

ISR(TIMER1_CAPT_vect) {
  ticks = ICR1;
  TCNT1 = 0; // reset timer
}

int main() {
  TCCR1A = 0;
  // enable noise canceler, rising edge, no prescaler
  TCCR1B = (0 << ICNC1) | (1 << ICES1) | (1 << CS10);
  TIMSK = (1 << TICIE1); // enable input capture intterupt flag

  serial_init(115200);
  sei();

  while (1) {
    char buff[30] = {};

    sprintf(buff, "period = %fs\n\r", ticks / ticks_per_second);
    serial_print(buff);
    _delay_ms(100);
  }
}

void serial_print(const char *str) {
  while (*str) {
    while (!(UCSRA & (1 << UDRE)))
      ;
    UDR = *(str++);
  }
}

void serial_init(const uint32_t baud) {
  const uint16_t ubrr = F_CPU / 16 / baud - 1;
  UBRRH = (uint8_t)(ubrr >> 8);
  UBRRL = (uint8_t)(ubrr);
  // Enable receiver and transmitter
  UCSRB = (1 << RXEN) | (1 << TXEN);
  // Set frame format: 8data, 1stop bit, no parity
  UCSRC = (1 << URSEL) | (0 << USBS) | (3 << UCSZ0);
}
