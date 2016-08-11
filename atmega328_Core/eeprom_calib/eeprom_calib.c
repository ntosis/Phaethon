#include <eeprom_calib.h>

// Create variable in EEPROM with initial values
CAL_PARAM EEMEM CALinEE = { 0,1.56,0.001,800,1.56,0.001,800,7,0};
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
/*! \brief P, I and D parameter values
*
* The K_P, K_I and K_D values (P, I and D gains)
* need to be modified to adapt to the application at hand
*/
