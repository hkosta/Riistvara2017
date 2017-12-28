#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <time.h>
#include <util/delay.h>
#include "print_helper.h"
#include "cli_microrl.h"
#include "hmi_msg.h"
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-­‐uart/uart.h"
#include "../lib/helius_microrl/ microrl.h"

#define UART_BAUD           9600
#define UART_STATUS_MASK    0x00FF

#define BLINK_DELAY_MS  2000
#define LED_RED         PORTA0 // Arduino Mega digital pin 22
#define LED_GREEN         PORTA2 // Arduino Mega digital pin 24
#define LED_BLUE         PORTA4 // Arduino Mega digital pin 26

//Create microrl object and pointer on it
microrl_t rl;
microrl_t *prl = &rl;

static inline void init_leds(void)
{
    /* RGB LED board set up and off */
    DDRA |= _BV(LED_RED) | _BV(LED_GREEN);
    PORTA &= ~(_BV(LED_RED) | _BV(LED_GREEN));
}

/* Init error console as stderr in UART1 and print user code info */
static inline void init_errcon(void)
{
    uart1_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart1_puts_p(PSTR("Console started\r\n"));
}

static inline void init_clicon(void)
{
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart0_puts_p((PGM_P)pgm_read_word(&stud_name[]));
    //Call init with ptr to microrl instance and print callback
    microrl_init(prl, uart0_puts);
    //Set callback for execute
    microrl_set_execute_callback(prl, cli_execute);
}

static inline void init_sys_timer(void)
{
    //    counter_1 = 0; // Set counter to random number 0x19D5F539 in HEX. Set it to 0 if you want
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12); // Turn on CTC (Clear Timer on Compare)
    TCCR1B |= _BV(CS12); // fCPU/256
    OCR1A = 62549; // Note that it is actually two registers OCR5AH and OCR5AL
    TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
}

//Süsteemi aja väljatrükiks ASCII märkidena saab kasutada ltoa() funktsiooni.
static inline void print_sys_time(void)
{
    // Note that print frequency depends from big program simulation duration.
    // See heartbeat snippet for how to print system time once per second.
    char iso_time[20] = {0x00};
    struct tm now_tm;
    time_t now = time(NULL);
    gmtime_r(&now, &now_tm);
    isotime_r(&now_tm, iso_time);
    uart1_puts(iso_time);
    uart1_puts_p(PSTR("\r\n"));
}

static inline void heartbeat(void)
{
    static time_t prev_time;
    char ascii_buf[11] = {0x00};
    time_t now = time(NULL);

    if (prev_time != now) {
        //Print uptime to uart1
        ltoa(now, ascii_buf, 10);
        uart1_puts_p(PSTR("Uptime: "));
        uart1_puts(ascii_buf);
        uart1_puts_p(PSTR(" s.\r\n"));
        //Toggle LED
        PORTA ^= _BV(LED_GREEN);
        prev_time = now;
    }
}

static inline void init_lcd(void) {
    lcd_init();
    lcd_home();
    lcd_puts_P(PSTR(NAME)); /*prindib ekraanile minu nime*/
    lcd_goto(LCD_ROW_2_START); /*liigub ekraanil järgmise rea algusesse*/
}

void main(void)
{
    init_sys_timer();
    init_leds();
    init_lcd();
    init_errcon();
    init_clicon();
    fprintf_P(stdout, PSTR(NAME)); /*prindib konsooli minu nime*/
    fprintf(stdout, "\r\n");

    while (1) {
        heartbeat();
        // CLI commands are handled in cli_execute()
        microrl_insert_char(prl, (uart0_getc() & UART_STATUS_MASK));
    }
}

/* Counter 1 ISR */
ISR(TIMER1_COMPA_vect)
{
    system_tick();
}
