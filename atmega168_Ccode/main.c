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
void drawTime(void);
void Set_Input(int16_t inputValue);
void showDebugInfo(void);
void showTimeNow(void);
extern u8g_t u8g;

static char buf[80];
static uint8_t SOLLtemperature = 22;
static int16_t inputValue;
struct PID_DATA pidData;
int main(void)
{
	int set;
	__OUTPUT(RED_LED);
	//__HIGH(FAST_RED_LED);
	_delay_ms(20);
	//TM1637DisplayInit();
	initTempSens();
	initRtrEncoder();
	u8g_setup();
	timerInit();
	pid_Init(K_P*SCALING_FACTOR,K_I*SCALING_FACTOR,K_D*SCALING_FACTOR,&pidData);
	// set up the task list
    initScheduler();
    //clearDisplay(0,4);
    //showTime();
	// add tasks, id is arbitrary
	// task1 runs every 800ms second
	addTask(1,ButtonAction, 8);
	// task2 runs every 700ms seconds
	addTask(2,updateSollTemperature,7);
	// task3 runs every 1 seconds
	addTask(3, PIDController, 10);
	// task4 runs every 1.5 seconds
	addTask(4, displayController, 15);
	//task5
	addTask(5, LEDfunction, 5);
	//task 6 autoProgram runs every a min
	addTask(6,autoProgram,600);
	//send command to relay
	addTask(7, commandToRelay,400);
	//addTask(7, showActualTemperature,10);
	//enable interrupts
	sei();
  while(1)
  {
	  	  //Task manager runs
	      dispatchTasks();

  }
}
void updateSollTemperature() {

	 if (TurnDetected&&onStateofProgram) {
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
	static uint8_t count=0;
	count++;
	switch(count) {
	case 1:
		showDebugInfo();
		break;
	case 3:
		showTimeNow();
		break;
		}
	if(count==4) count=0;
}

void u8g_setup(void)
{
  _delay_ms(5);
  u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_FAST);
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
  sprintf(buf,"P%.1f:I%.1f:D%.1f",0.5,K_I,K_D);
  u8g_DrawStr(&u8g, 0,50,buf);
  sprintf(buf,"PID= %d",inputValue);
  u8g_DrawStr(&u8g, 0,60,buf);
  sprintf(buf,"Relay= %d",stateOfHeatingSystem);
  u8g_DrawStr(&u8g, 65,10,buf);
  sprintf(buf,"Day= %d",GetDoW());
  u8g_DrawStr(&u8g, 65,20,buf);
  int T = roundf(actualTemperature());
  sprintf(buf,"Tem/re= %d",T);
  u8g_DrawStr(&u8g, 65,30,buf);
  sprintf(buf,"Soll/Te= %d",SOLLtemperature);
  u8g_DrawStr(&u8g, 61,40,buf);
}
void showDebugInfo(void) {
	u8g_FirstPage(&u8g);
		    do
		    {
		      draw();
		    } while ( u8g_NextPage(&u8g) );
}
void showTimeNow(void) {
	u8g_FirstPage(&u8g);
			    do
			    {
			      drawTime();
			    } while ( u8g_NextPage(&u8g) );
}
void drawTime(void){
	u8g_SetFont(&u8g,u8g_font_fub17);

	sprintf(buf,"%d:%02d",GetHH(),GetMM());
	u8g_DrawStr(&u8g, 0,25,buf);
	sprintf(buf,"%02d/%02d/%4d",GetDD(),GetMonth(),2000+GetYY());
	u8g_DrawStr(&u8g, 0,60,buf);

}
