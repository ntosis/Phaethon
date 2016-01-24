// MACROS FOR EASY PIN HANDLING FOR ATMEL GCC-AVR
#ifndef _PINMACROS_H_
#define _PINMACROS_H_
//these macros are used indirectly by other macros , mainly for string concatination
#include <avr/io.h>
#include <stdint.h>
typedef enum { false, true } bool;
#define _SET(type,name,bit)          type ## name  |= _BV(bit)
#define _CLEAR(type,name,bit)        type ## name  &= ~ _BV(bit)
#define _TOGGLE(type,name,bit)       type ## name  ^= _BV(bit)
#define _GET(type,name,bit)          ((type ## name >> bit) &  1)
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )

//these macros are used by end user
#define __OUTPUT(pin)         _SET(DDR,pin)
#define __INPUT(pin)          _CLEAR(DDR,pin)
#define __HIGH(pin)           _SET(PORT,pin)
#define __LOW(pin)            _CLEAR(PORT,pin)
#define __TOGGLE(pin)         _TOGGLE(PORT,pin)
#define __READ(pin)           _GET(PIN,pin)
#define FAST_RED_LED C,1
#define GREEN_LED C,2
#define BLUE_LED C,0
#define RELAY_PIN D,6
#define PIN0 D,0
#endif
