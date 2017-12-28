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
//#define COUNT_1_10_SECONDS  // 1/10 seconds
//#define COUNT_1_100_SECONDS // 1/100 seconds

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

/* Init error console as stderr in UART1 and print user code info */
static inline void init_errcon(void)
{
    uart1_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart1_puts_p(PSTR("Console started\r\n"));
}

static inline void init_clicon(void)
{
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart0_puts_p(stud_name);
    
    //Call init with ptr to microrl instance and print callback
    microrl_init(prl, uart0_puts);
    //Set callback for execute
    microrl_set_execute_callback(prl, cli_execute);
}

static inline void init_counter_1(void)
{
    counter_1 = 433452345; // Set counter to random number 0x19D5F539 in HEX. Set it to 0 if you want
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12); // Turn on CTC (Clear Timer on Compare)
#ifdef COUNT_SECONDS
    TCCR1B |= _BV(CS12); // fCPU/256
    OCR1A = 62549; // Note that it is actually two registers OCR5AH and OCR5AL
#endif /* COUNT_SECONDS */
#ifdef COUNT_1_10_SECONDS
    TCCR1B |= _BV(CS12); // fCPU/256
    OCR1A = 6249;
#endif /* COUNT_1_10_SECONDS */
#ifdef COUNT_1_100_SECONDS
    TCCR1B |= _BV(CS10) | _BV(CS11); // fCPU/64
    OCR1A = 2499;
#endif /* COUNT_1_100_SECONDS */
    TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
}

static inline void heartbeat(void)
{
    uint32_t counter_1_cpy = 0;
#ifdef ASCII_PRINT
    char ascii_buf[11] = {0x00}; // Buffer lagre enough to hold all long (uint32_t) digits
#endif /* ASCII_PRINT */
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        counter_1_cpy = counter_1;
    }
    // FIXME Help! How can we print something human readable now?
    // Where is my stdout and printf() formatting???
#ifdef ASCII_PRINT
    // Here you have something
    ltoa(counter_1_cpy, print_buf, 10);
    uart1_puts(ascii_buf);
    uart1_puts_p(PSTR("\r\n"));
#else
    // Send bytes. Byte 3 first.
    uart1_putc((uint8_t) (counter_1_cpy >> 24));    // Put counter byte 3
    uart1_putc((uint8_t) (counter_1_cpy >> 16));    // Put counter byte 2
    uart1_putc((uint8_t) (counter_1_cpy >> 8));     // Put counter byte 1
    uart1_putc((uint8_t) counter_1_cpy);            // Put counter byte 0
#endif /* ASCII_PRINT */
}

static inline void init_lcd(void) {
    lcd_init();
    lcd_home();
    /*lcd_puts_P(PSTR(NAME)); prindib ekraanile minu nime
    lcd_goto(LCD_ROW_2_START); liigub ekraanil järgmise rea algusesse*/
}

void main(void)
{
    init_counter_1();
    init_leds();
    init_lcd();
    init_errcon();
    init_clicon();
    /*fprintf_P(stdout, PSTR(NAME)); prindib konsooli minu nime
    fprintf(stdout, "\r\n");*/

    while (1) {
        heartbeat();
        // CLI commands are handled in cli_execute()
        microrl_insert_char(prl, (uart0_getc() & UART_STATUS_MASK));
    }
}
