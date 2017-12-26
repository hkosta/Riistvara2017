#include <avr/pgmspace.h>
#ifndef HMI_MSG_H_
#define HMI_MSG_H_

#define VER_FW "Version: " FW_VERSION " built on: " __DATE__ " " __TIME__ "\n"
#define VER_LIBC "avr-libc version: " __AVR_LIBC_VERSION_STRING__ " avr-gcc version: " __VERSION__ "\n"


#define ENTER_NUMBER "\nEnter number > "
#define INPUT_PRINT "\nYou entered number: "
#define ERROR "\nPlease enter number between 0 and 9!\n"
#define ERROR_LCD "Please enter number between 0 and 9!"

#define NAME "Helen Kosta"

const char n0[] PROGMEM = "Zero";
const char n1[] PROGMEM = "One";
const char n2[] PROGMEM = "Two";
const char n3[] PROGMEM = "Three";
const char n4[] PROGMEM = "Four";
const char n5[] PROGMEM = "Five";
const char n6[] PROGMEM = "Six";
const char n7[] PROGMEM = "Seven";
const char n8[] PROGMEM = "Eight";
const char n9[] PROGMEM = "Nine";

PGM_P const numbers[] PROGMEM = {
                                n0,
                                n1,
                                n2,
                                n3, 
                                n4, 
                                n5, 
                                n6, 
                                n7, 
                                n8, 
                                n9
                                };

#endif /* HMI_MSG_H_ */

