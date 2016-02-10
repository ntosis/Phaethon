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
void inline SetDate(uint8_t dd, uint8_t mo, uint8_t yy);
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
int GetDD();
int GetMonth();
int GetYY();
static void _draw(uint8_t idx);
static char buf[80];
extern u8g_t u8g;
static int8_t hour;
static int8_t minute;
static int8_t day;
static uint8_t month;
static uint8_t calendarDay;
static uint16_t year=2016;
static char *daysInWords[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
static char *monthsInWords[] = {"January","February","March","April","May","June","July","August","September","October","November","December"};

void inline SetTime(int8_t hh, int8_t mm, int8_t dd){

	//set Sec
	uint8_t sec =0x00;
	i2c_writeReg(ds1307_adr,0x00,&sec,1);
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
void inline SetDate(uint8_t dd, uint8_t mo, uint8_t yy){

	//set the Hour
	dd = dec2bcd(dd);
	i2c_writeReg(ds1307_adr,0x04,&dd,1);
	//set the minute
	mo = dec2bcd(mo);
	i2c_writeReg(ds1307_adr,0x05,&mo,1);
	//set the day
	yy = dec2bcd(yy-2000);
	i2c_writeReg(ds1307_adr,0x06,&yy,1);

}
#endif
