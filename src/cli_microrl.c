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
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "../lib/matejx_avr_lib/spi.h"
#include "../lib/andy_brown_memdebug/memdebug.h"

#define NUM_ELEMS(x) (sizeof(x) / sizeof((x)[0]))

void cli_print_help(const char *const *argv);
void cli_example(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_number(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
void cli_rfid_read(const char *const *argv);
//void rfid_card_add(const char *const *argv);
//void rfid_card_remove(const char *const *argv);
//void rfid_card_print_list(void);
//void rfid_process_card(void);
//void rfid_handle_door_and_disp(void);
void cli_mem_stat(const char *const *argv);

typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;

typedef struct rfid_card_list {
    struct rfid_card *next;
    uint8_t uid_size;
    char *name;
    uint8_t uid[10];
    uint8_t sak;
} rfid_card_t;

//static rfid_card_t *head = NULL;

const char help_cmd[] PROGMEM = "help";
const char help_help[] PROGMEM = "Get help";
const char example_cmd[] PROGMEM = "example";
const char example_help[] PROGMEM = "Prints out all provided 3 arguments Usage: example <argument> <argument> <argument>";
const char ver_cmd[] PROGMEM = "version";
const char ver_help[] PROGMEM = "Print FW version";
const char ascii_cmd[] PROGMEM = "ascii";
const char ascii_help[] PROGMEM = "Print ASCII tables";
const char number_cmd[] PROGMEM = "number";
const char number_help[] PROGMEM = "Print and display matching number Usage: number <decimal number>";
const char mem_stat_cmd[] PROGMEM = "mem";
const char mem_stat_help[] PROGMEM = "Print memory usage and change compared to previous call";
const char read_cmd[] PROGMEM = "read";
const char read_help[] PROGMEM = "Reads a RFID card and displays its data";

const cli_cmd_t cli_cmds[] = {
    {read_cmd, read_help, cli_rfid_read, 0},
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {example_cmd, example_help, cli_example, 3},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {number_cmd, number_help, cli_handle_number, 1},
    {mem_stat_cmd, mem_stat_help, cli_mem_stat, 0},
};

void cli_rfid_read(const char *const *argv)
{
    (void) argv;
    Uid uid;
    Uid *uid_ptr = &uid;
    
    byte bufferATQA[10];
    byte bufferSize = sizeof(bufferATQA);
    byte result;
    result = PICC_WakeupA(bufferATQA, &bufferSize);

if (result != STATUS_OK) {
    if (PICC_IsNewCardPresent()){
        uart0_puts(PSTR("Card selected!\r\n"));
        PICC_ReadCardSerial(uid_ptr);
        printf("UID size: 0x%02X\r\n", uid.size);
        printf("UID sak: 0x%02X\r\n", uid.sak);
        uart0_puts_p(PSTR("Card UID: "));
        for (byte i = 0; i < uid.size; i++){
            printf("%02X", uid.uidByte[i]);
        }

        uart0_puts_p(PSTR("\r\n"));
    } else {
        uart0_puts_p((PSTR("Unable to select card.\r\n")));
    }
    }
}

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
    (void) argv;

    for (size_t i = 0; i < strlen(argv[1]); i++) {
        if (!isdigit(argv[1][i])) {
            uart0_puts_p(PSTR("Argument is not a decimal number!\r\n"));
            return;
        }
    }

    int input = atoi(argv[1]);

    if ( input >= 0 && input <= 9) {
        lcd_clrscr();
        uart0_puts_p(PSTR("Your entered number "));
        uart0_puts_p((PGM_P)pgm_read_word(
                         &numbers[input])); /*prindib sisestatud numbrile vastava väärtuse sõnede massiivist*/
        uart0_puts_p(PSTR("\r\n"));
        lcd_puts_P((PGM_P)pgm_read_word(&numbers[input]));
    } else {
        lcd_clrscr();
        uart0_puts_p(
            PSTR("Please enter number between 0 and 9!\r\n")); /*hoiatus vale sisestuse puhul*/
        lcd_puts_P(PSTR("Please enter number between 0 and 9!\r\n"));
    }
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

void cli_mem_stat(const char *const *argv)
{
    (void) argv;
    char print_buf[256] = {0x00};
    extern int __heap_start, *__brkval;
    int v;
    int space;
    static int prev_space;
    space = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    uart0_puts_p(PSTR("Heap statistics\r\n"));
    sprintf_P(print_buf, PSTR("Used: %u\r\nFree: %u\r\n"), getMemoryUsed(),
              getFreeMemory());
    uart0_puts(print_buf);
    uart0_puts_p(PSTR("\r\nSpace between stack and heap:\r\n"));
    sprintf_P(print_buf, PSTR("Current  %d\r\nPrevious %d\r\nChange   %d\r\n"),
              space, prev_space, space - prev_space);
    uart0_puts(print_buf);
    uart0_puts_p(PSTR("\r\nFreelist\r\n"));
    sprintf_P(print_buf, PSTR("Freelist size:             %u\r\n"),
              getFreeListSize());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Blocks in freelist:        %u\r\n"),
              getNumberOfBlocksInFreeList());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest block in freelist: %u\r\n"),
              getLargestBlockInFreeList());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest freelist block:    %u\r\n"),
              getLargestAvailableMemoryBlock());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest allocable block:   %u\r\n"),
              getLargestNonFreeListBlock());
    uart0_puts(print_buf);
    prev_space = space;
}

int cli_execute(int argc, const char *const *argv)
{
    // Move cursor to new line. Then user can see what was entered.
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

