
//Note: 1. Define Clock in Configuration Opetions
//      2. Define RTC Connections in i2c.h
#include "ds1307.h"
#include <avr/io.h>
#include "i2c.h"
#define ds1307_adr 0B11010000
char buff[8];
char mystr[8];
int temp;
#define I2C_START 0
#define I2C_DATA  1
#define I2C_STOP  2
#define MAX_TRIES 50
//=================================================================
//                    RTC1307_READ_WRITE                         //
//=================================================================


uint8_t Read_RTC(char add)
{
	i2c_readReg(ds1307_adr,add,buff,1);
	return buff[0];
}
int Write_RTC(char add,char data1)
{
return 0;
}

//=======================================================================
//                           SET TIME
//=======================================================================
void SetTime(char HH,char MM, char SS, char ampm)
{
	sprintf(mystr,"%03d",SS);
	Write_RTC(0x00,((mystr[1] - 0x30) << 4) | (mystr[2] - 0x30));

	sprintf(mystr,"%03d",MM);
	Write_RTC(0x01,((mystr[1] - 0x30) << 4) | (mystr[2] - 0x30));


	sprintf(mystr,"%03d",HH);
	if(ampm == 1)
	{
		Write_RTC(0x02,((((mystr[1] - 0x30) << 4) | (mystr[2] - 0x30)) | 0x40) | 0x20);
    }
	else
	{
		Write_RTC(0x02,((((mystr[1] - 0x30) << 4) | (mystr[2] - 0x30)) | 0x40));
	}
}
//=======================================================================
int GetHH()
{
	uint8_t j= Read_RTC(0x02);
	if ((j & 0x40) == 0x40) {
		 return bcd2dec(j & 0x1F);
	   } else {
	     return bcd2dec(j & 0x3F);
	   }
}
//=======================================================================
int GetMM()
{
	uint8_t j= (Read_RTC(0x01) & 0x7F);
	return bcd2dec(j);
}
//=======================================================================
int GetSS()
{
	uint8_t j= Read_RTC(0x00);
	j=j& 0x7F;
	return  bcd2dec(j);
}
//=======================================================================
//                   SET DATE
//=======================================================================
void SetDate(char DD,char MM, char YY)
{
	sprintf(mystr,"%03d",DD);
	Write_RTC(0x04,((mystr[1] - 0x30) << 4)  | (mystr[2] - 0x30));

	sprintf(mystr,"%03d",MM);
	Write_RTC(0x05,((mystr[1] - 0x30) << 4) | (mystr[2] - 0x30));

	sprintf(mystr,"%03d",YY);
	Write_RTC(0x06,((mystr[1] - 0x30) << 4)  | (mystr[2] - 0x30));
}
//=======================================================================
int GetDD()
{
	return bcd2dec(Read_RTC(0x04));
}
int GetDoW()
{
	return bcd2dec(Read_RTC(0x03));
}
//=======================================================================
int GetMonth()
{
	return bcd2dec(Read_RTC(0x05)); //12/11
}
//=======================================================================
int GetYY()
{
	return bcd2dec(Read_RTC(0x06));
}
// Convert Binary Coded Decimal (BCD) to Decimal
uint8_t bcd2dec(uint8_t num)
{
  return ((num/16 * 10) + (num % 16));
}
//=======================================================================
//                    Display Date and Time
//=======================================================================
void DisplayDateTime()
{
		temp = Read_RTC(0x00);

		mystr[7]=48+(temp & 0b00001111);
		mystr[6]=48+((temp & 0b01110000)>>4);
		mystr[5]=':';

		temp = Read_RTC(0x01);

		mystr[4]=48+(temp & 0b00001111);
		mystr[3]=48+((temp & 0b01110000)>>4);
		mystr[2]=':';

		temp = Read_RTC(0x02);

		mystr[1]=48+(temp & 0b00001111);
		mystr[0]=48+((temp & 0b00010000)>>4);
}
void showTime() {
	clearDisplay(0,4);
	if(GetHH()<10){showNumberDec(0, false, 1, 0);}
	showNumberDec(GetHH(), false, 2, 0);
	if(GetMM()<10){
		showNumberDec(0, false, 1, 2);
		showNumberDec(GetMM(), false, 1,3);
	}
else {
	showNumberDec(GetMM(), false, 2,2);
	}
}
