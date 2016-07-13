#include "heatingSys.h"

bool autoProgramSelected=false;
bool manualProgramSelected=false;
bool autoProgramTimeEnabled=false;
bool stateOfHeatingSystem=false;

// On/Off time Array
// Sunday->Saturday ontime/offtime
  const uint16_t onTimesAM[7] PROGMEM =
  //ontime
  {845,550,550,550,550,550,845};

  const uint16_t offTimesAM[7] PROGMEM =
  //offtime
  {1200,645,645,645,645,645,1200};

  const uint16_t onTimesPM[7] PROGMEM =
  //ontime
  {1300,1630,1630,1630,1630,1630,1400} ;

  const uint16_t offTimesPM[7] PROGMEM =
  //offtime
  {2200,2200,2200,2200,2200,2300,2300};

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
void LEDfunction() {
	if(!stateOfProgram) {
		__LOW(RED_LED);
		__HIGH(GREEN_LED);
	}
	else if((stateOfProgram)&&(!selectProgram)){
		__HIGH(RED_LED);
		__LOW(GREEN_LED);
	}
	else if((stateOfProgram)&&(selectProgram)) {
		__TOGGLE(GREEN_LED);
		__HIGH(RED_LED);
	}
	if(Ctrl_Subsystem_Y.Out1==true) {
		__TOGGLE(BLUE_LED);
	}
	else if(Ctrl_Subsystem_Y.Out1==false) {
		__HIGH(BLUE_LED);
	}
}
uint16_t returnDebugInfo() {
	return GetHH()*100 +GetMM();

}
