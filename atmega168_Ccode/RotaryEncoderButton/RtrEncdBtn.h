#ifndef _RTRENCDBTN_H_
#define _RTRENCDBTN_H_

#include "pinmacros.h"
#include <avr/interrupt.h>
#include "timerInterrupt.h"
#define PinCLK D,2                   // Used for generating interrupts using CLK signal
#define PinDT D,4                    // Used for reading DT signal
#define PinSW D,3
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
