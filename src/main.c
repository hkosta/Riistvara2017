#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "print_helper.h"
#include "cli_microrl.h"
#include "hmi_msg.h"
#include <util/atomic.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"

#define UART_BAUD           9600
#define UART_STATUS_MASK    0x00FF

#define LED_RED         PORTA0 // Arduino Mega digital pin 22
#define LED_GREEN         PORTA2 // Arduino Mega digital pin 24
#define LED_BLUE         PORTA4 // Arduino Mega digital pin 26

#define COUNT_SECONDS       // seconds

#include <stdlib.h> // stdlib is needed to use ltoa() - Long to ASCII

/* Global seconds counter */
volatile uint32_t counter_1;


//Create microrl object and pointer on it
microrl_t rl;
microrl_t *prl = &rl;

static inline void init_leds(void)
{
    /* RGB LED board set up and off */
    DDRA |= _BV(LED_RED) | _BV(LED_GREEN);
    PORTA &= ~(_BV(LED_RED) | _BV(LED_GREEN));
}

/* Init error console as stderr in UART1 and print user code in fo */
static inline void init_errcon(void)
{
    uart1_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart1_puts_p(PSTR(VER_FW));
    uart1_puts_p(PSTR("Console started\r\n"));
}

static inline void init_clicon(void)
{
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart0_puts_p(stud_name);
    uart0_puts_p(
        PSTR("\r\nUse backspace to delete entry and enter to confirm.\r\n"));
    uart0_puts_p(PSTR("Arrow key's and del do not work currently.\r\n"));
    //Call init with ptr to microrl instance and print callback
    microrl_init(prl, uart0_puts);
    //Set callback for execute
    microrl_set_execute_callback(prl, cli_execute);
}

static inline void init_counter_1(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12); // Turn on CTC (Clear Timer on Compare)
#ifdef COUNT_SECONDS
    TCCR1B |= _BV(CS12); // fCPU/256
    OCR1A = 62549; // Note that it is actually two registers OCR5AH and OCR5AL
#endif /* COUNT_SECONDS */
    TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
}

static inline void heartbeat(void)
{
    static uint32_t counter_1_prev;
    char ascii_buf[11] = {0x00}; // Buffer lagre enough to hold all long (uint32_t) digits
    uint32_t counter_1_cpy = counter_1;

    if (counter_1_cpy != counter_1_prev) {
        ltoa(counter_1_cpy, ascii_buf, 10);
        uart1_puts_p(PSTR("Uptime: "));
        uart1_puts(ascii_buf);
        uart1_puts_p(PSTR(" s\r\n"));
        //Toggle LED
        PORTA ^= _BV(LED_RED);
        ATOMIC_BLOCK(ATOMIC_FORCEON) {
            counter_1_cpy = counter_1_prev;
        }
    }
}

static inline void init_lcd(void)
{
    lcd_init();
    lcd_home();
    lcd_puts_P(stud_name); /*prindib ekraanile minu nime*/
    lcd_goto(LCD_ROW_2_START); /*liigub ekraanil järgmise rea algusesse*/
}

void main(void)
{
    init_leds();
    init_lcd();
    init_errcon();
    init_clicon();
    init_counter_1();
    sei();

    while (1) {
        heartbeat();
        // CLI commands are handled in cli_execute()
        microrl_insert_char(prl, (uart0_getc() & UART_STATUS_MASK));
    }
}

ISR(TIMER1_COMPA_vect)
{
    counter_1++;
}
