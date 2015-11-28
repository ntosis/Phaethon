#ifndef _TEMPERATURESENS_H_
#define _TEMPERATURESENS_H_

#include <avr/delay.h>
#include "spi.h"
//slave select
#define Sensor_SS       D,5

void initTempSens();
float actualTemperature();

#endif
