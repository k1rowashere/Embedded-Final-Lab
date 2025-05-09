#include <avr/io.h>
#include <util/delay.h>

int16_t analog_read(const uint8_t admux) {
  ADMUX = (ADMUX & 0xF0) | (admux & 0x0F); // set ADC channel
  _delay_us(10);
  ADCSRA |= (1 << ADSC); // start conversion
  while (ADCSRA & (1 << ADSC))
    ; // wait for conversion to complete
  return ADC;
}

int main() {
  // led pin
  DDRB = (1 << PB0);

  ADMUX = (1 << REFS0); // AVcc with external capacitor at AREF pin
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (0b111 << ADPS0);

  while (1) {
    int16_t d1 = analog_read(0) - 500; // ADC00
    int16_t d2 = analog_read(1) - 500; // ADC01
    uint32_t th = d1 * d1 + d2 * d2;

    if (th >= 50 * 50)
      PORTB |= (1 << PB0); // turn on LED
    else
      PORTB &= ~(1 << PB0); // turn off LED
  }
}