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
    uint8_t  cycles;    // Cycles in buffer
    int8_t   amp;       // Amplitude range: [-127,0]
    uint8_t  type;      // Waveform type
    uint16_t duty;      // Duty cycle range: [0,512]
    int8_t   offset;    // Offset
    uint32_t desiredF;  // Desired frequency
} CAL_PARAM;

void LoadCALvars(void);
void SaveCALvars(void);

extern CAL_PARAM CALinRAM;

#define cycles CALinRAM.cycles
#define duty CALinRAM.duty
#define type CALinRAM.type
#endif /* EEPROM_CALIB_H_ */
