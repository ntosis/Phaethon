/*
 * eeprom_calib.h
 *
 *  Created on: Jul 5, 2016
 *      Author: unix
 */

#ifndef EEPROM_CALIB_H_
#define EEPROM_CALIB_H_
#include <pinmacros.h>
#include <avr/eeprom.h>
// Create structure
typedef struct {
    uint8_t  cycles;
    float K_P;
    float K_I;
    float K_D;
} CAL_PARAM;

void LoadCALvars(void);
void SaveCALvars(void);

extern CAL_PARAM CALinRAM;

#define cycles CALinRAM.cycles
#define K_P CALinRAM.K_P
#define K_I CALinRAM.K_I
#define K_D CALinRAM.K_D

#endif /* EEPROM_CALIB_H_ */

