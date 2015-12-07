/*
 * heatingsys.h
 *
 *  Created on: Nov 27, 2015
 *      Author: unix
 */

#ifndef HEATINGSYS_H_
#define HEATINGSYS_H_

#include <avr/pgmspace.h>
#include "pinmacros.h"
#include "ds1307.h"
#include "RtrEncdBtn.h"
#include "tm1637.h"

void autoProgram();
void setStateOfHeatingSystem(bool state);
void LEDfunction(void);
void flashAutoFunctionIfIsOn(void);
uint16_t returnDebugInfo();
extern bool autoProgramSelected;
extern bool manualProgramSelected;
extern bool autoProgramTimeEnabled;
extern bool stateOfHeatingSystem;

#endif /* HEATINGSYS_H_ */
