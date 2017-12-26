#include <stdio.h>
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
    fprintf_P(stderr, PSTR(VER_LIBC));
    fprintf_P(stderr, PSTR(VER_FW));
}

static inline void init_clicon(void)
{
    simple_uart0_init(); /*initsialiseerib uart0*/
    stdout = stdin = &simple_uart0_io;
}

static inline void blink_leds(void)
{
    PORTB &= ~_BV(PORTB7);
    PORTA |= _BV(PORTA0);
    _delay_ms(BLINK_DELAY_MS);
    PORTA &= ~_BV(PORTA0);
    _delay_ms(BLINK_DELAY_MS);
    PORTA |= _BV(PORTA2);
    _delay_ms(BLINK_DELAY_MS);
    PORTA &= ~_BV(PORTA2);
    _delay_ms(BLINK_DELAY_MS);
    PORTA |= _BV(PORTA4);
    _delay_ms(BLINK_DELAY_MS);
    PORTA &= ~_BV(PORTA4);
    _delay_ms(BLINK_DELAY_MS);
}



void main(void)
{
    init_leds();
    init_errcon();
    init_clicon();
    lcd_init();
    lcd_home();
    lcd_puts_P(PSTR(NAME)); /*prindib ekraanile minu nime*/
    lcd_goto(LCD_ROW_2_START); /*liigub ekraanil järgmise rea algusesse*/
    fprintf_P(stdout, PSTR(NAME)); /*prindib konsooli minu nime*/
    fprintf(stdout, "\n");
    print_ascii_tbl(stdout); /*prindib konsooli ASCII tabeli*/
    unsigned char ascii[128] = {0}; /*loob massiivi ja täidab selle väärtustega 0-127*/

    for (unsigned char i = 0; i < 128; i++) {
        ascii[i] = i;
    }

    print_for_human(stdout, ascii, 128);

    while (1) {
        blink_leds();
        int s; /*initsialiseerib inputi*/
        fprintf_P(stdout, PSTR(ENTER_NUMBER));
        fscanf(stdin, "%d", &s); /*võtab klaviatuurisisestuse sisse*/
        fprintf(stdout, "%d", s); /*prindib sisestatud numbri*/

        if ( s >= 0 && s <= 9) {
            lcd_clrscr();
            fprintf_P(stdout, PSTR(INPUT_PRINT));
            fprintf_P(stdout, PSTR("%S"),
                      (PGM_P)pgm_read_word(&numbers[s])); /*prindib sisestatud numbrile vastava väärtuse sõnede massiivist*/
            lcd_puts_P((PGM_P)pgm_read_word(&numbers[s]));
            stderr;
        } else {
            lcd_clrscr();
            fprintf_P(stdout, PSTR(ERROR)); /*hoiatus vale sisestuse puhul*/
            lcd_puts_P(PSTR(ERROR_LCD));
            stderr;
        }
    }
}
