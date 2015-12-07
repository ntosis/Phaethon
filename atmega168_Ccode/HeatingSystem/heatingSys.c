#include "heatingSys.h"

bool autoProgramSelected=false;
bool manualProgramSelected=false;
bool autoProgramTimeEnabled=false;
bool stateOfHeatingSystem=false;
const uint8_t  SEG_0A[] = {
  SEG_A|SEG_B|SEG_C|SEG_G|SEG_E|SEG_F   // A
  };
// On/Off time Array
// Sunday->Saturday ontime/offtime
  const uint16_t onTimesAM[7] PROGMEM =
  //ontime
  {845,615,615,615,615,615,845};

  const uint16_t offTimesAM[7] PROGMEM =
  //offtime
  {1200,705,705,705,705,705,1200};

  const uint16_t onTimesPM[7] PROGMEM =
  //ontime
  {1300,1630,1630,1630,1630,1630,1400} ;

  const uint16_t offTimesPM[7] PROGMEM =
  //offtime
  {2130,2230,2230,2230,2230,2330,2200};

void autoProgram() {


  uint16_t convertedTime= GetHH()*100 +GetMM();

  uint16_t onTimeAM= pgm_read_word_near(onTimesAM + (GetDoW()-1));
  uint16_t offTimeAM= pgm_read_word_near(offTimesAM + (GetDoW()-1));
  uint16_t onTimePM= pgm_read_word_near(onTimesPM + (GetDoW()-1));
  uint16_t offTimePM= pgm_read_word_near(offTimesPM + (GetDoW()-1));

  if((onTimeAM<convertedTime&&convertedTime<offTimeAM)||(onTimePM<convertedTime&&convertedTime<offTimePM)) {
	  autoProgramTimeEnabled=true;

  }
  else
    {
	  autoProgramTimeEnabled=false;
  }

}
void setStateOfHeatingSystem(bool state) {
	stateOfHeatingSystem = state;

}

void LEDfunction() {
	if(!onStateofProgram) {
		__HIGH(FAST_RED_LED);
	}
	else {
		__LOW(FAST_RED_LED);
	}
	if(manualProgramSelected&&(autoProgramSelected==0)) {
		__HIGH(GREEN_LED);
	}
	else if(autoProgramSelected&&(manualProgramSelected==0)) {
		__TOGGLE(GREEN_LED);
	}
	if(stateOfHeatingSystem==true) {
		__TOGGLE(BLUE_LED);
	}
	else if(stateOfHeatingSystem==false) {
		__LOW(BLUE_LED);
	}
}
void flashAutoFunctionIfIsOn() {

	if(autoProgramSelected) {
		clearDisplay(0,2);
		setSegments(SEG_0A,1,1);
		}
	}
uint16_t returnDebugInfo() {
	return GetHH()*100 +GetMM();

}
