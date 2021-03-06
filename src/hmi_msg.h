#ifndef HMI_MSG_H_
#define HMI_MSG_H_

#define VER_FW "Version: " FW_VERSION " built on: " __DATE__ " " __TIME__ "\r\n"
#define VER_LIBC "avr-libc version: " __AVR_LIBC_VERSION_STRING__ " avr-gcc version: " __VERSION__ "\r\n"

extern const char stud_name[];
extern const char version[];
extern const char avr_version[];
extern PGM_P const numbers[];

#endif /* HMI_MSG_H_ */
