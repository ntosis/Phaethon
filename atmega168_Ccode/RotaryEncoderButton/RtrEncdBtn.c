#include "RtrEncdBtn.h"
#include <stdlib.h>
#include "heatingSys.h"
bool clicked=false;
bool doubleClicked=false;
bool onStateofProgram=false;
uint8_t holdCnt=0;
extern volatile uint8_t signalButton = 0;
extern volatile bool up = false;
extern volatile bool TurnDetected = false;
void initRtrEncoder() {

	__INPUT(PinCLK);
	__INPUT(PinDT);
	//disable internal pull ups
	__LOW(PinCLK);
	__LOW(PinDT);
	attacheInterrupt();

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

    ArrayOfClicks[pnt].timeOfClick=counter100MS;
    ArrayOfClicks[pnt].clicked=true;
    pnt++;
         //
    if(pnt==5) pnt=0;
    if(signalButton<2) signalButton++;
}
void checkStruct() {

  cli();
  pnt=0;
 clicked=false;
 doubleClicked=false;
  for(int i=0;i<5;i++) {
         if(i<4){
         if(ArrayOfClicks[i].clicked==true)  {
             if(ArrayOfClicks[i+1].clicked==true) {
            	int8_t dt= (ArrayOfClicks[i+1].timeOfClick)-(ArrayOfClicks[i].timeOfClick);
               if(dt<7) {
            	   doubleClicked=true;
                   //jump to next incoming signal.
            	   i++;
            	   continue;
                }
               else clicked=true;
             }
             else clicked=true;
         }
       }
       else if(i==4) {
         if(ArrayOfClicks[i].clicked==true)  {
        	if(ArrayOfClicks[0].clicked==true) {
               int8_t dt= (ArrayOfClicks[0].timeOfClick)-(ArrayOfClicks[i].timeOfClick);
               dt=abs(dt);
               if(dt<7) {
            	   doubleClicked=true;
               //i++;
            	   continue;
             }
               else clicked=true;

             }
             else clicked=true;
         }
       }
  }
    for(int i=0;i<5;i++) {
      ArrayOfClicks[i].clicked=false;
      ArrayOfClicks[i].timeOfClick=0;
    }
   sei();
}
uint8_t returnStateofProgram() {return onStateofProgram;}
