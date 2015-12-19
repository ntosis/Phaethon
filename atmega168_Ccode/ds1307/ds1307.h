#ifndef DS1307_H
#define DS1307_H

#include <avr/io.h>
#include <string.h>
#include <avr/delay.h>
#include "tm1637.h"

void SetTime(char HH,char MM, char SS, char ampm);
int GetHH();
int GetMM();
int GetSS();
int GetDoW();
void SetDate(char DD,char MM, char YY);
int GetDD();
int GetMonth();
int GetYY();
uint8_t bcd2dec(uint8_t num);
uint8_t Read_RTC(char add);
int Write_RTC(char add,char data1);
void showTime();
#endif
