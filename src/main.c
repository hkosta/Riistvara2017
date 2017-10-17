#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000

void main (void)
{
    /* Set port A pin 0 for output for Arduino Mega yellow LED */
    DDRA |= _BV(DDA0);

    while (1) {
        /* Set port A pin 0 high to turn Arduino Mega yellow LED on */
        PORTA |= _BV(PORTA0);
        _delay_ms(BLINK_DELAY_MS);
        /* Set port A pin 0 high to turn Arduino Mega yellow LED off */
        PORTA &= ~_BV(PORTA0);
        _delay_ms(BLINK_DELAY_MS);
    }
}
