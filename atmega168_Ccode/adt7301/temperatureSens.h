#ifndef _TEMPERATURESENS_H_
#define _TEMPERATURESENS_H_

#include <avr/delay.h>
#include "spi.h"

void initTempSens();
float actualTemperature();
extern  uint8_T Temperature;
#endif