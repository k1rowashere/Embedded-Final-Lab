#include <avr/interrupt.h>
#include <avr/io.h>

ISR(ADC_vect) {
  PORTC = ADC >> 2;
  TIFR &= (1 << OCF1B); // clear interrupt flag
}

int main() {
  // PORTC => output
  DDRC = 0xFF;
  DDRD = (1 << PD4);

  // timer1 1s
  TCCR1B = (1 << WGM12) | (0b100 << CS10); // CTC, prescaler = 256
  OCR1B = 43200;                           // 1s
  OCR1A = 43200;                           // 1s

  // ADC auto-trigger every 1s, prescaler = 128
  ADCSRA =
      (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADIE) | (0b111 << ADPS0);
  SFIOR = (0b101 << ADTS0); // ADC trigger on timer 1 compare match
  ADMUX = 0;                // ADC0
  sei();

  while (true)
    ;
}