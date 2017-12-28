#include <avr/pgmspace.h>
#ifndef HMI_MSG_H_
#define HMI_MSG_H_

#define VER_FW "Version: " FW_VERSION " built on: " __DATE__ " " __TIME__ "\r\n"
#define VER_LIBC "avr-libc version: " __AVR_LIBC_VERSION_STRING__ " avr-gcc version: " __VERSION__ "\r\n"


/*#define ENTER_NUMBER "\r\nEnter number > "
#define INPUT_PRINT "\r\nYou entered number: "
#define ERROR "\r\nPlease enter number between 0 and 9!\r\n"
#define ERROR_LCD "Please enter number between 0 and 9!"*/

extern const char stud_name[];
extern PGM_P const numbers[];

#endif /* HMI_MSG_H_ */

