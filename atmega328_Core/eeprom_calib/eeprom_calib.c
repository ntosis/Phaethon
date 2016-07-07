#include <eeprom_calib.h>

// Create variable in EEPROM with initial values
CAL_PARAM EEMEM CALinEE = { 1, -65, 55, 256, 0, 44000 };
// Create variable in RAM
CAL_PARAM  CALinRAM;

// Load data from EEPROM
void LoadCALvars(void) {

	eeprom_read_block(&CALinRAM, &CALinEE, sizeof(CAL_PARAM));
}
// Save data to EEPROM
void SaveCALvars(void) {

	eeprom_write_block(&CALinRAM, &CALinEE, sizeof(CAL_PARAM));
}
