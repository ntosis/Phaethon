#ifndef DS1307_H
#define DS1307_H

#include <avr/io.h>
#include <string.h>
#include <avr/delay.h>
#include "tm1637.h"
#include "u8g.h"

#define ds1307_adr 0B11010000

extern volatile uint8_t signalButton;
void inline SetTime(int8_t hh, int8_t mm, int8_t dd);
uint8_t bcd2dec(uint8_t num);
uint8_t dec2bcd(uint8_t num);
uint8_t Read_RTC(char add);
int Write_RTC(char add,char data1);
void showTime();
void setTimeLoop(void);
int GetHH();
int GetMM();
int GetSS();
int GetDoW();
void SetDate(char DD,char MM, char YY);
int GetDD();
int GetMonth();
int GetYY();
static void _draw(uint8_t idx);
static char buf[80];
extern u8g_t u8g;
static int8_t hour=0;
static int8_t minute=0;
static int8_t day=1;

static char *daysInWords[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

void inline SetTime(int8_t hh, int8_t mm, int8_t dd){

	//set the Hour
	hh = dec2bcd((uint8_t)hh);
	i2c_writeReg(ds1307_adr,0x02,&hh,1);
	//set the minute
	mm = dec2bcd((uint8_t)mm);
	i2c_writeReg(ds1307_adr,0x01,&mm,1);
	//set the day
	dd = dec2bcd((uint8_t)dd);
	i2c_writeReg(ds1307_adr,0x03,&dd,1);

}
#endif
