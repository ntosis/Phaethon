#include <avr/io.h>
#include <avr/delay.h>
#include "math.h"
#include "temperatureSens.h"
#include "RtrEncdBtn.h"
#include "timerInterrupt.h"
#include "Ctrl_Subsystem.h"
#include "heatingSys.h"
#include "u8g.h"
#include <avr/pgmspace.h>
#include <eeprom_calib.h>
#include <stdio.h>
#include <OLED_graphics.h>
//#include "pinmacros.h"

void updateSollTemperature();
void ButtonAction(void);
void displayController(void);
void commandToRelay(void);
void u8g_setup(void);
void draw(void);
void drawTime(void);
void drawSollTemp(void);
void showDebugInfo(void);
void showTimeNow(void);
void showSolltemp(void);
extern u8g_t u8g;
extern pidData_t pidData_Htng;
extern pidData_t pidData_Coolg;
extern int8_t SOLLtemperature;
extern int16_t inputValue_Htng;
extern int16_t inputValue_Coolg;
extern ExtY_Ctrl_Subsystem Ctrl_Subsystem_Y;
extern boolean_T isHeatingOn;
char buf[80];

int main(void)
{
	LoadCALvars();

	int set;
	__OUTPUT(RED_LED);
	__OUTPUT(GREEN_LED);
	__OUTPUT(BLUE_LED);

	_delay_ms(20);
	//TM1637DisplayInit();
	initTempSens();
	initRtrEncoder();
	u8g_setup();
	timerInit();
	pid_Init(K_P_Htng*SCALING_FACTOR,K_I_Htng*SCALING_FACTOR,K_D_Htng*SCALING_FACTOR,&pidData_Htng);
	pid_Init(K_P_Coolg*SCALING_FACTOR,K_I_Coolg*SCALING_FACTOR,K_D_Coolg*SCALING_FACTOR,&pidData_Coolg);
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
	addTask(3, Ctrl_Subsystem_step, 10);
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

	 if (TurnDetected&&stateOfProgram) {
  if (up)
      SOLLtemperature--;
    else
      SOLLtemperature++;
    showSolltemp();
 }
     TurnDetected = false;    // do NOT repeat IF loop until new rotation detected

}
void ButtonAction(void) {
	checkStruct();
	checkHoldButton(); //inline function to check hold buton.();
}
//The pin have to be HIGH to turn off the heating system
void commandToRelay() {
	if(Ctrl_Subsystem_Y.Out1) {
		__LOW(RELAY_PIN);
	}
	//If the Thermostat is in OFF state means that we have two turn off the heating system completely.
	else if(!Ctrl_Subsystem_Y.Out1) {
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
  bool aut,man;
  if(selectProgram) aut=1,man=0;
  else aut=0,man=1;
  sprintf_P(buf,PSTR("StOnOff= %d"),stateOfProgram);
  u8g_SetFont(&u8g, u8g_font_6x10);
  u8g_DrawStr(&u8g, 0, 10,buf);
  sprintf_P(buf,PSTR("Man= %d"),man);
  u8g_DrawStr(&u8g, 0,20,buf);
  sprintf_P(buf,PSTR("Aut= %d"),aut);
  u8g_DrawStr(&u8g, 0,30,buf);
  sprintf_P(buf,PSTR("TmEnb= %d"),autoProgramTimeEnabled);
  u8g_DrawStr(&u8g, 0,40,buf);
  sprintf_P(buf,PSTR("PID_H= %d"),inputValue_Htng);
  u8g_DrawStr(&u8g, 0,50,buf);
  sprintf_P(buf,PSTR("PID_C= %d"),inputValue_Coolg);
  u8g_DrawStr(&u8g, 0,60,buf);
  sprintf_P(buf,PSTR("Relay= %d"),Ctrl_Subsystem_Y.Out1);
  u8g_DrawStr(&u8g, 65,10,buf);
  sprintf_P(buf,PSTR("Day= %d"),GetDoW());
  u8g_DrawStr(&u8g, 65,20,buf);
  int T = roundf(actualTemperature());
  sprintf_P(buf,PSTR("Tem/re=%d"),T);
  u8g_DrawStr(&u8g, 65,30,buf);
  sprintf_P(buf,PSTR("Soll/Te=%d"),SOLLtemperature);
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

	u8g_SetFont(&u8g,u8g_font_9x18r);
	sprintf_P(buf,PSTR("%d:%02d"),GetHH(),GetMM());
	u8g_DrawStr(&u8g, 64,15,buf);

	u8g_SetFont(&u8g,u8g_font_9x18r);
	sprintf_P(buf,PSTR("%02d/%02d/%4d"),GetDD(),GetMonth(),2000+GetYY());
	u8g_DrawStr(&u8g, 15,62,buf);

	int T = roundf(actualTemperature());
	u8g_SetFont(&u8g,u8g_font_10x20);
	sprintf_P(buf,PSTR("%d\260C"),T);
	u8g_DrawStr(&u8g, 55,41,buf);

	if(isHeatingOn) {
		u8g_DrawXBMP(&u8g,5, 5,flame_width, flame_height, flame_bits);
	}
	else {
		u8g_DrawXBMP(&u8g,10, 10,snowflake_width, snowflake_height, nifada_xioniou_bits);
	}
}

void showSolltemp(void) {
	u8g_FirstPage(&u8g);
		do
		    {
		      drawSollTemp();
		    } while ( u8g_NextPage(&u8g) );
	}
void drawSollTemp(void) {
	uint8_t temp=SOLLtemperature;
	 u8g_DrawCircle(&u8g, 64, 32, 17, U8G_DRAW_ALL);
	 u8g_SetFont(&u8g,u8g_font_fub17);
	 if(SOLLtemperature<0) temp=temp*(-1);
	 sprintf(buf,"%d",temp);
	 u8g_DrawStr(&u8g,64-13,32+7,buf);
}

