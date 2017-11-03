#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 800

void main (void)
{
    /* Set port B pin 7 for output for Arduino Mega yellow LED*/
    DDRB |= _BV(DDB7);

    /* Set port A pin 0 for output for RGB LED red light*/
    DDRA |= _BV(DDA0);

    /* Set port A pin 2 for output for RGB LED blue light*/
    DDRA |= _BV(DDA2);

    /* Set port A pin 4 for output for RGB LED green light*/
    DDRA |= _BV(DDA4);

    while (1) {
        /* Set port B pin 7 high to turn Arduino Mega yellow LED off */
        PORTB &= ~_BV(PORTB7);

        /* Set port A pin 0 high to turn RGB LED red light on */
        PORTA |= _BV(PORTA0);
        _delay_ms(BLINK_DELAY_MS);
        /* Set port A pin 0 high to turn RGB LED red light off */
        PORTA &= ~_BV(PORTA0);
        _delay_ms(BLINK_DELAY_MS);

        /* Set port A pin 2 high to turn RGB LED blue light on */
        PORTA |= _BV(PORTA2);
        _delay_ms(BLINK_DELAY_MS);
        /* Set port A pin 2 high to turn RGB LED blue light off */
        PORTA &= ~_BV(PORTA2);
        _delay_ms(BLINK_DELAY_MS);

        /* Set port A pin 4 high to turn RGB LED green light on */
        PORTA |= _BV(PORTA4);
        _delay_ms(BLINK_DELAY_MS);
        /* Set port A pin 4 high to turn RGB LED blue light off */
        PORTA &= ~_BV(PORTA4);
        _delay_ms(BLINK_DELAY_MS);
    }
}
