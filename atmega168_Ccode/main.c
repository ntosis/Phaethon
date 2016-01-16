#include <avr/io.h>
#include <avr/delay.h>
#include "math.h"
#include "tm1637.h"
#include "temperatureSens.h"
#include "RtrEncdBtn.h"
#include "timerInterrupt.h"
#include "pid.h"
#include "heatingSys.h"
#include "u8g.h"
//#include "pinmacros.h"
/*! \brief P, I and D parameter values
*
* The K_P, K_I and K_D values (P, I and D gains)
* need to be modified to adapt to the application at hand
*/
//! \xrefitem todo "Todo" "Todo list"
#define K_P     10
//! \xrefitem todo "Todo" "Todo list"
#define K_I     0.5
//! \xrefitem todo "Todo" "Todo list"
#define K_D     2

void updateSollTemperature();
void showActualTemperature(void);
void ButtonAction(void);
void displayController(void);
void PIDController(void);
void commandToRelay(void);
uint8_t Get_Reference(void);
uint8_t Get_Measurement(void);
void u8g_setup(void);
void draw(void);
void Set_Input(int16_t inputValue);
u8g_t u8g;

char buf[80];
static uint8_t SOLLtemperature = 22;
static int16_t inputValue;
struct PID_DATA pidData;
int main(void)
{
	int set;
	__OUTPUT(FAST_RED_LED);

	//__HIGH(FAST_RED_LED);
	_delay_ms(500);
	  u8g_setup();
	TM1637DisplayInit();
	initTempSens();
	initRtrEncoder();
	timerInit();
	pid_Init(K_P*SCALING_FACTOR,K_I*SCALING_FACTOR,K_D*SCALING_FACTOR,&pidData);
	// set up the task list
    initScheduler();
    clearDisplay(0,4);
    showTime();
	// add tasks, id is arbitrary
	// task1 runs every 800ms second
	addTask(1,ButtonAction, 8);
	// task2 runs every 700ms seconds
	addTask(2,updateSollTemperature,7);
	// task3 runs every 1 seconds
	addTask(3, PIDController, 10);
	// task4 runs every 4 seconds
	addTask(4, displayController, 40);
	//task5
	addTask(5, LEDfunction, 5);
	//task 6 autoProgram runs every a min
	addTask(6,autoProgram,600);
	//send command to relay
	addTask(7, commandToRelay,400);
	addTask(7, showActualTemperature,10);
	//enable interrupts
	sei();
  while(1)
  {
	  	  //Task manager runs
	      dispatchTasks();

  }
}
void updateSollTemperature() {

	 if (TurnDetected) {
  if (up)
      SOLLtemperature--;
    else
      SOLLtemperature++;
     if(SOLLtemperature<5) SOLLtemperature=5;
    if(SOLLtemperature>40) SOLLtemperature=40;
    clearDisplay(0,4);
    showNumberDec(SOLLtemperature, false, 2, 0);
 }
     TurnDetected = false;    // do NOT repeat IF loop until new rotation detected

}
void showActualTemperature(void) {
	clearDisplay(0,4);
	int T = roundf(actualTemperature());
	showNumberDec(T, false, 2,2);
	u8g_FirstPage(&u8g);
	    do
	    {
	      draw();
	    } while ( u8g_NextPage(&u8g) );
}
void ButtonAction(void) {
	checkStruct();
	checkIfButtonIsPressed();
}
void PIDController() {
		if(autoProgramSelected&&autoProgramTimeEnabled){
			inputValue = pid_Controller(Get_Reference(), Get_Measurement(), &pidData);
			Set_Input(inputValue);
		}
		else {pid_Reset_Integrator(&pidData);}
}
/*! \brief Read reference value.
 *
 * This function must return the reference value.
 * May be constant or varying
 */
uint8_t Get_Reference()
{
  return SOLLtemperature;
}
/*! \brief Read system process value
 *
 * This function must return the measured data
 */
uint8_t Get_Measurement()
{
  return roundf(actualTemperature());
}
/*! \brief Set control input to system
 *
 * Set the output from the controller as input
 * to system.
 */
void Set_Input(int16_t inputValue)
{
  if(inputValue<=0) setStateOfHeatingSystem(false);
else if(inputValue>0) setStateOfHeatingSystem(true);
 // showNumberDec(inputValue, false, 4,0);
}

//The pin have to be HIGH to turn off the heating system
void commandToRelay() {
	if(manualProgramSelected&&onStateofProgram) {
		if(SOLLtemperature - (roundf(actualTemperature()))>0) {
		    setStateOfHeatingSystem(true);
		    __LOW(RELAY_PIN);
		}
	    else {
	    	setStateOfHeatingSystem(false);
	    	__HIGH(RELAY_PIN);
	    }
	}
	else if(autoProgramSelected&&onStateofProgram) {
		//The PID controller calls the setStateOfHeatingSystem function
		if(stateOfHeatingSystem) {
			__LOW(RELAY_PIN);
		}
		else if(!stateOfHeatingSystem) {
			__HIGH(RELAY_PIN);
		}
		}
	//If the Thermostat is in OFF state means that we have two turn off the heating system completely.
	else if(!onStateofProgram) {
		__HIGH(RELAY_PIN);
	}
}
void displayController() {
	static count=0;
	count++;
	switch(count) {
	case 2:
		//showActualTemperature();
		break;
	case 1:
		showTime();
		break;
	case 3:
		flashAutoFunctionIfIsOn();
		break;
		}
	if(count==3) count=0;
}

void u8g_setup(void)
{

  u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_DEV_1);

}
void draw(void)
{
  sprintf(buf,"StOnOff= %d",onStateofProgram);
  u8g_SetFont(&u8g, u8g_font_6x10);
  u8g_DrawStr(&u8g, 0, 10,buf);
  sprintf(buf,"Man= %d",manualProgramSelected);
  u8g_DrawStr(&u8g, 0,20,buf);
  sprintf(buf,"Aut= %d",autoProgramSelected);
  u8g_DrawStr(&u8g, 0,30,buf);
  sprintf(buf,"TmEnb= %d",autoProgramTimeEnabled);
  u8g_DrawStr(&u8g, 0,40,buf);
  sprintf(buf,"Time= %d:%d",GetHH(),GetMM());
  u8g_DrawStr(&u8g, 0,50,buf);
  sprintf(buf,"PID= %d",inputValue);
  u8g_DrawStr(&u8g, 0,60,buf);
  sprintf(buf,"Relay= %d",stateOfHeatingSystem);
  u8g_DrawStr(&u8g, 65,10,buf);
  sprintf(buf,"Day= %d",GetDoW());
  u8g_DrawStr(&u8g, 65,20,buf);
  sprintf(buf,"Tem/re= %d",roundf(actualTemperature()));
  u8g_DrawStr(&u8g, 65,30,buf);
  sprintf(buf,"Soll/Te= %d",SOLLtemperature);
    u8g_DrawStr(&u8g, 61,40,buf);
}
