#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define __ASSERT_USE_STDERR
#include <assert.h>
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "print_helper.h"
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"

#define BLINK_DELAY_MS 100


static inline void init_leds(void)
{
    /* Set port B pin 7 for output for Arduino Mega yellow LED*/
    DDRB |= _BV(DDB7);
    /* Set port A pin 0 for output for RGB LED red light*/
    DDRA |= _BV(DDA0);
    /* Set port A pin 2 for output for RGB LED blue light*/
    DDRA |= _BV(DDA2);
    /* Set port A pin 4 for output for RGB LED green light*/
    DDRA |= _BV(DDA4);
}


/* Init error console as stderr in UART1 and print user code info */
static inline void init_errcon(void)
{
    simple_uart1_init();
    stderr = &simple_uart1_out;
    fprintf(stderr, "Version: %s built on: %s %s\n",
            FW_VERSION, __DATE__, __TIME__);
    fprintf(stderr, "avr-libc version: %s avr-gcc version: %s\n",
            __AVR_LIBC_VERSION_STRING__, __VERSION__);
}


static inline void blink_leds(void)
{
    /* lab2 LEDs blink code goes here */
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

void main(void)
{
    init_leds();
    init_errcon();
    lcd_init();
    lcd_home();
    lcd_puts(strPtr);
    lcd_goto(LCD_ROW_2_START);
    simple_uart0_init();
    stdin = stdout = &simple_uart0_io;
    fprintf (stdout, strPtr);
    fprintf (stdout, "\n");
    /*fprintf(stdout, stud_name, "\n");*/
    print_ascii_tbl(stdout);
    unsigned char ascii[128] = {0};

    for (unsigned char i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128);

    while (1) {
        blink_leds();
        int s;
        printf("Enter number > ");
        fscanf(stdin, "%d", &s);
        printf("%d", s);

        if ( s >= 0 && s <= 9) {
            printf("\nYou entered number %s\n", numbers[s]);
            lcd_puts(numbers[s]);
            exit(0);
        } else {
            printf("\nPlease enter number between 0 and 9! \n");
            lcd_puts("Please enter number between 0 and 9!");
            exit(1);
        }
    }
}

