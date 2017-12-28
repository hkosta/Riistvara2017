#include <avr/pgmspace.h>
#include "hmi_msg.h"

const char stud_name[] PROGMEM = "Helen Kosta";

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
