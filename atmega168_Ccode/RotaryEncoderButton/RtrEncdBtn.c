#include "RtrEncdBtn.h"
#include <stdlib.h>
#include "heatingSys.h"

bool onStateofProgram=false;

void initRtrEncoder() {

	__INPUT(PinCLK);
	__INPUT(PinDT);
	//disable internal pull ups
	__LOW(PinCLK);
	__LOW(PinDT);
	attacheInterrupt();

	ArrayOfClicks = (click*)malloc(5*sizeof(click));

}

void attacheInterrupt() {

	EICRA |= (1 << ISC01)|(1<<ISC11);    //  The falling edge of INT0 & INT1 generates an interrupt request
	EIMSK |= (1 << INT0)|(1<<INT1);     // Turns on INT0 & INT1
}

ISR (INT0_vect)
{
	 if (__READ(PinCLK))
	    up = __READ(PinDT);
	  else
	    up = !(__READ(PinDT));
	  TurnDetected = true;
}

ISR(INT1_vect) {

    //Serial.println(millis());      // Using the word RESET instead of COUNT here to find out a buggy encoder

    (ArrayOfClicks+pnt)->timeOfClick=counter100MS;
    (ArrayOfClicks+pnt)->clicked=true;
     pnt++;
         //
    if(pnt==5) pnt=0;
}
void checkStruct() {

  cli();
 clicked=false;
 dualclicked=false;
  for(int i=0;i<5;i++) {
         if(i<4){
         if((ArrayOfClicks+i)->clicked==true)  {
             if((ArrayOfClicks+i+1)->clicked==true) {
               int dt= ((ArrayOfClicks+i+1)->timeOfClick)-((ArrayOfClicks+i)->timeOfClick);
               if(dt<7) {
                 dualclicked=true;
                 //jump to next incoming signal.
                  i++;
                }
               else clicked=true;

             }
             else clicked=true;
         }
       }
       else if(i==4) {
         if((ArrayOfClicks+i)->clicked==true)  {
        	if((ArrayOfClicks)->clicked==true) {
               int8_t dt= ((ArrayOfClicks)->timeOfClick)-((ArrayOfClicks+i)->timeOfClick);
               dt=abs(dt);
               if(dt<7) {
               dualclicked=true;
               //i++;
             }
               else clicked=true;

             }
             else clicked=true;
         }
       }
  }
    for(int i=0;i<5;i++) {
      (ArrayOfClicks+i)->clicked=false;
      (ArrayOfClicks+i)->timeOfClick=0;
    }
   sei();
}

bool checkIfButtonIsPressed() {

  if(clicked)
  {
   if(!onStateofProgram) {
   onStateofProgram=true;
   manualProgramSelected=true;
   return true;
      }
   else {
   onStateofProgram=false;
   manualProgramSelected=false;
   autoProgramSelected=false;
   return false;
      }
   }
  else if(onStateofProgram&&dualclicked) {
	  autoProgramSelected=(!autoProgramSelected);
	  manualProgramSelected=(!manualProgramSelected);
  }
}
void setOnOffState(bool a) {
      // ON
      // autoProgramm
        if(a&&dualclicked) {
        // call the Auto Program
        autoProgramSelected=(!autoProgramSelected);

        }
        else if(!a) {
        	autoProgramSelected=false;
        	manualProgramSelected=false;
        }
        else if(a&&(!dualclicked)) {
        	manualProgramSelected=(!manualProgramSelected);
        }

}
uint8_t returnStateofProgram() {return onStateofProgram;}
