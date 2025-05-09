#include <avr/io.h>

int main() {
  DDRD = (1 << PD5); // PD5 output
  // f = 50kHz, duty = 7.5%
  // phase correct pwm, top = ICR1
  // N (prescaler) = 1
  // ICR1 = F_CPU / (N * f * 2) - 1 = 109.592.
  // OCR1A =  top * duty = 110 * 7.5% = 8.25
  TCCR1A = (0b10 << COM1A0) | (1 << WGM11);
  TCCR1B = (1 << WGM13) | (1 << CS10);
  ICR1 = 110;
  OCR1A = 8;

  while (true)
    ;
}