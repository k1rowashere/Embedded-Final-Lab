#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

void serial_print(const char *str);
void serial_init(uint32_t baud);

ISR(TIMER1_COMPA_vect) {
  char buff[20] = {};
  snprintf(buff, sizeof(buff), "PORTB = %u\r\n", PINB);
  serial_print(buff);
}

int main() {
  DDRB = 0;     // Set PORTB as input
  PORTB = 0xFF; // Enable pull-up resistors on PORTB

  serial_init(9600);

  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (0b101 << CS10); // CTC mode, prescaler 1024
  OCR1A = 21600;                           // 2s
  TIMSK = (1 << OCIE1A);                   // Enable Timer1 compare interrupt
  sei();                                   // Enable global interrupts

  while (true)
    ;
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
