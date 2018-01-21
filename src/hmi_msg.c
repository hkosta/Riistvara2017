#include <avr/pgmspace.h>
#include "hmi_msg.h"

const char stud_name[] PROGMEM = "Helen Kosta";
const char version[] PROGMEM = "Version: " FW_VERSION " built on: " __DATE__ " "
                               __TIME__ "\r\n";
const char avr_version[] PROGMEM = "avr-libc version: "
                                   __AVR_LIBC_VERSION_STRING__ " avr-gcc version: " __VERSION__ "\r\n";


const char n0[] PROGMEM = "zero";
const char n1[] PROGMEM = "one";
const char n2[] PROGMEM = "two";
const char n3[] PROGMEM = "three";
const char n4[] PROGMEM = "four";
const char n5[] PROGMEM = "five";
const char n6[] PROGMEM = "six";
const char n7[] PROGMEM = "seven";
const char n8[] PROGMEM = "eight";
const char n9[] PROGMEM = "nine";

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
