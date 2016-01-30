#ifndef _RTRENCDBTN_H_
#define _RTRENCDBTN_H_

#include "pinmacros.h"
#include <avr/interrupt.h>
#include "timerInterrupt.h"
#include "ds1307.h"
/*
 * The ports for the pins are defined in pinmacros.h
 * The PD2 PD3 PD 4 are used for the rotary encoder
 */
void initRtrEncoder();
void inline attacheInterrupt(void);
void inline checkHoldButton(void);
void checkStruct();
bool checkIfButtonIsPressed(void);
uint8_t returnStateofProgram();

extern volatile bool TurnDetected;
extern volatile bool up;

typedef struct {
long int timeOfClick;
bool clicked;
} click;

static click *ArrayOfClicks;
static bool clicked=false;
static bool dualclicked=false;
extern bool onStateofProgram;
extern volatile uint8_t signalButton; // not to be optimized
volatile static uint8_t pnt=0;
uint8_t holdCnt;

void inline attacheInterrupt(void) {

	EICRA |= (1 << ISC01)|(1<<ISC11)|(1<<ISC10);    //  The rising edge of INT1 and falling edges from INT0 generates an interrupt request
	EIMSK |= (1 << INT0)|(1<<INT1);     // Turns on INT0 & INT1
}

void inline checkHoldButton(void) {

	if(!__READ(PinSW)) {  			//if button is logical LOW

		holdCnt++;

	}

	else if(__READ(PinSW)) {

		holdCnt=0;

	}

	if(holdCnt>4) setTimeLoop(); //this task is called every 800ms, for 3 sec we calculate a value ~ 4
}

#endif
