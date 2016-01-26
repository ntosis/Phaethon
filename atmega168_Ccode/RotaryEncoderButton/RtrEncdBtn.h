#ifndef _RTRENCDBTN_H_
#define _RTRENCDBTN_H_

#include "pinmacros.h"
#include <avr/interrupt.h>
#include "timerInterrupt.h"
/*
 * The ports for the pins are defined in pinmacros.h
 * The PD2 PD3 PD 4 are used for the rotary encoder
 */
void initRtrEncoder();
void attacheInterrupt();
void checkStruct();
bool checkIfButtonIsPressed();
uint8_t returnStateofProgram();
volatile bool TurnDetected;
volatile bool up;
typedef struct {
long int timeOfClick;
bool clicked;
} click;

static click *ArrayOfClicks;
static bool clicked=false;
static bool dualclicked=false;
extern bool onStateofProgram;
volatile static uint8_t pnt=0;

#endif
