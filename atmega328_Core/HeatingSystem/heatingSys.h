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

void autoProgram();
void setStateOfHeatingSystem(bool state);
void LEDfunction(void);
uint16_t returnDebugInfo();
extern boolean_T stateOfProgram; /* '<S5>/XOR1' */
extern boolean_T selectProgram; /* '<S2>/XOR1' */
extern bool autoProgramTimeEnabled;
extern boolean_T stateOfRelay; /* '<S1>/Logical Operator' */



#endif /* HEATINGSYS_H_ */
