
//Note: 1. Define Clock in Configuration Opetions
//      2. Define RTC Connections in i2c.h
#include "ds1307.h"

char mystr[8];
int temp;

//=================================================================
//                    RTC1307_READ_WRITE                         //
//=================================================================
uint8_t Read_RTC(char add)
{
	uint8_t temp1;
	I2C_START_TX(0b11010000);
	i2c_transmit(add);
	i2c_start();
	I2C_START_RX(0b11010000);
	temp1 = i2c_receive(0);
	i2c_stop();
	return(temp1);
}

int Write_RTC(char add,char data1)
{
	I2C_START_TX(0b11010000);	//device add.
    i2c_transmit(add);	//Reg. add.
	i2c_transmit(data1);
	i2c_stop();
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
