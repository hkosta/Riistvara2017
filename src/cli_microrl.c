//TODO There are most likely unnecessary includes. Clean up during lab6
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "hmi_msg.h"
#include "cli_microrl.h"
#include "print_helper.h"
#include "main.c"

#define NUM_ELEMS(x)        (sizeof(x) / sizeof((x)[0]))

void cli_print_help(const char *const *argv);
void cli_example(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_number(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);


typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;

const char help_cmd[] PROGMEM = "help";
const char help_help[] PROGMEM = "Get help";
const char example_cmd[] PROGMEM = "example";
const char example_help[] PROGMEM =
    "Prints out all provided 3 arguments Usage: example <argument> <argument> <argument>";
const char ver_cmd[] PROGMEM = "version";
const char ver_help[] PROGMEM = "Print FW version";
const char ascii_cmd[] PROGMEM = "ascii";
const char ascii_help[] PROGMEM = "Print ASCII tables";
const char number_cmd[] PROGMEM = "number";
const char number_help[] PROGMEM =
    "Print and display matching number Usage: number <decimal number>";


const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {example_cmd, example_help, cli_example, 3},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {number_cmd, number_help, cli_handle_number, 1},
};


void cli_print_help(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(PSTR("Implemented commands:\r\n"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        uart0_puts_p(cli_cmds[i].cmd);
        uart0_puts_p(PSTR(" : "));
        uart0_puts_p(cli_cmds[i].help);
        uart0_puts_p(PSTR("\r\n"));
    }
}

void cli_example(const char *const *argv)
{
    uart0_puts_p(PSTR("Command had following arguments:\r\n"));

    for (uint8_t i = 1; i < 4; i++) {
        uart0_puts(argv[i]);
        uart0_puts_p(PSTR("\r\n"));
    }
}

void cli_print_ver(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(PSTR(VER_FW));
    uart0_puts_p(PSTR(VER_LIBC));
}


void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    print_ascii_tbl(); /*prindib konsooli ASCII tabeli*/
    unsigned char ascii[128] = {0}; /*loob massiivi ja täidab selle väärtustega 0-127*/

    for (unsigned char i = 0; i < 128; i++) {
        ascii[i] = i;
    }
    
    print_for_human(ascii, 128);
}

void cli_handle_number(const char *const *argv)
{
    // TODO remove those two lines after command is implemented
    (void) argv;
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
    uart0_puts_p(
        PSTR("Command not implemented yet.\r\n\tImplement it by yourself!\r\n"));
}


void cli_print_cmd_error(void)
{
    uart0_puts_p(PSTR("Command not implemented.\r\n\tUse <help> to get help.\r\n"));
}


void cli_print_cmd_arg_error(void)
{
    uart0_puts_p(
        PSTR("To few or too many arguments for this command\r\n\tUse <help>\r\n"));
}


int cli_execute(int argc, const char *const *argv)
{
    // Move cursor to new line. Then user can see what was entered.
    //FIXME Why microrl does not do it?

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            // Test do we have correct arguments to run command
            // Function arguments count shall be defined in struct
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            // Hand argv over to function via function pointer,
            // cross fingers and hope that funcion handles it properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }

    cli_print_cmd_error();
    return 0;
}
