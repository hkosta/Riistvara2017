#ifndef HMI_MSG_H_
#define HMI_MSG_H_

#define VER_FW "Version: " FW_VERSION " built on: " __DATE__ " " __TIME__
#define VER_LIBC "avr-libc version: " __AVR_LIBC_VERSION_STRING__ " avr-gcc version: " __VERSION__

#define ENTER_NUMBER "Enter number > "
#define INPUT_PRINT "\nYou entered number %s\n"
#define ERROR "\nPlease enter number between 0 and 9!\n"
#define ERROR_LCD "Please enter number between 0 and 9!"

char *strPtr = "Helen Kosta"; 
char strArr[4] = {'S', 't', 'r', '\0'};

const char *numbers[] = {"Zero", "One",
                        "Two", "Three",
                        "Four", "Five",
                        "Six", "Seven",
                        "Eight", "Nine"};

#endif /* HMI_MSG_H_ */

