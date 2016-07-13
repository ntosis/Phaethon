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
    uint8_t  oneLevelSystem_C;
    float K_P_Htng;
    float K_I_Htng;
    float K_D_Htng;
    float K_P_Coolg;
    float K_I_Coolg;
    float K_D_Coolg;
} CAL_PARAM;

void LoadCALvars(void);
void SaveCALvars(void);

extern CAL_PARAM CALinRAM;

#define cycles CALinRAM.cycles
#define K_P_Htng CALinRAM.K_P_Htng
#define K_I_Htng CALinRAM.K_I_Htng
#define K_D_Htng CALinRAM.K_D_Htng
#define K_P_Coolg CALinRAM.K_P_Coolg
#define K_I_Coolg CALinRAM.K_I_Coolg
#define K_D_Coolg CALinRAM.K_D_Coolg
#define oneLevelSystem_C CALinRAM.oneLevelSystem_C
#endif /* EEPROM_CALIB_H_ */

