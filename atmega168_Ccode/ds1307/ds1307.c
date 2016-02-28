
//Note: 1. Define Clock in Configuration Opetions
//      2. Define RTC Connections in i2c.h
#include "ds1307.h"
#include <avr/io.h>

char buff[8];
char mystr[8];
int temp;
u8g_t u8g;
bool up;
bool TurnDetected;

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
// Convert Decimal to Binary Coded Decimal (BCD)

uint8_t dec2bcd(uint8_t num)
{
return ((num/10 * 16) + (num % 10));
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
/*
 * SET TIME LOOP
 * */
void setTimeLoop(void) {
	uint8_t tempValue = TCCR1B;//Disable the time interrupt to freeze the task manager

	TCCR1B=0x00; // Stop the timer
	hour = GetHH();
	minute = GetMM();
	day = GetDoW();
	month  = GetMonth() ;
	calendarDay  = GetDD();
	signalButton = 0; // Reset the signal, which is coming from external INT1

	do {

		u8g_FirstPage(&u8g);
				do{

			 _draw(0);

				} while ( u8g_NextPage(&u8g) );

		if (TurnDetected) {

			if (up)
			      hour--;
			    else
			      hour++;
			     if(hour<0) hour = 23;
			    if(hour>23) hour = 0;
			 }
	TurnDetected = false;    // do NOT repeat IF loop until new rotation detected

	_delay_ms(400);

	}while(signalButton<2);

	// ***************  Second step for minute

	signalButton = 0; // Reset the signal, which is coming from external INT1

	do {

		u8g_FirstPage(&u8g);
				do{

			 _draw(1);

				} while ( u8g_NextPage(&u8g) );

		if (TurnDetected) {

			if (up)
			      minute--;
			    else
			      minute++;
			     if(minute<0) minute = 59;
			    if(minute>59) minute = 0;
			 }
	TurnDetected = false;    // do NOT repeat IF loop until new rotation detected

	_delay_ms(400);

	}while(signalButton<1);

	// ***************  third step for day

		signalButton = 0; // Reset the signal, which is coming from external INT1

		do {

			u8g_FirstPage(&u8g);
					do{

				 _draw(2);

					} while ( u8g_NextPage(&u8g) );

			if (TurnDetected) {

				if (up)
				      day--;
				    else
				      day++;
				     if(day<1) day=7;
				    if(day>7) day=1;
				 }
		TurnDetected = false;    // do NOT repeat IF loop until new rotation detected

		_delay_ms(400);

		}while(signalButton<1);

		// ***************  fourth step for calendar day

				signalButton = 0; // Reset the signal, which is coming from external INT1

				do {

					u8g_FirstPage(&u8g);
							do{

						 _draw(3);

							} while ( u8g_NextPage(&u8g) );

					if (TurnDetected) {

						if (up)
						      calendarDay--;
						    else
						    	calendarDay++;
						     if(calendarDay<1) calendarDay=31;
						    if(calendarDay>31) calendarDay=1;
						 }
				TurnDetected = false;    // do NOT repeat IF loop until new rotation detected

				_delay_ms(400);

				}while(signalButton<1);

				// ***************  Fifth step for month

				signalButton = 0; // Reset the signal, which is coming from external INT1

				do {

				     u8g_FirstPage(&u8g);
					  do{

					   _draw(3);

				      } while ( u8g_NextPage(&u8g) );

			      if (TurnDetected) {

					if (up)
					 month--;
					else
					 month++;
					if(month<1) month=12;
						 if(month>12) month=1;
					}
					TurnDetected = false;    // do NOT repeat IF loop until new rotation detected

				_delay_ms(400);

			 }while(signalButton<1);

				// ***************  sixth step for year

					signalButton = 0; // Reset the signal, which is coming from external INT1

					do {

						u8g_FirstPage(&u8g);
						  do{

						   _draw(3);

					      } while ( u8g_NextPage(&u8g) );

					 if (TurnDetected) {

						if (up)
						 year--;
						else
							year++;
						if(year<2016) year=2016;
							 if(year>2099) year=2016;
						}
						TurnDetected = false;    // do NOT repeat IF loop until new rotation detected

						_delay_ms(400);

					}while(signalButton<1);

	// inline function to write the time to ds1307

		SetTime(hour,minute,day);
		SetDate(calendarDay,month,year);

	TCCR1B = tempValue; //Restore the timer.

	return;
}
void _draw(uint8_t idx)
{
	u8g_SetFont(&u8g, u8g_font_6x10);
	uint8_t y_pos=10;
	if(idx==0) {

			sprintf(buf,"Set hour = %02d",hour);

	}
	else if(idx==1) {

		    sprintf(buf,"Set minute = %02d",minute);

	}
	else if(idx==2) {

			if(day>7) return; // Boundary error check
			sprintf(buf,"Set day = %s",daysInWords[day-1]);

		}
	else if(idx==3) {

				if(day>7) return; // Boundary error check
				y_pos=30;
				u8g_DrawStr(&u8g, 0, 10,"Set date");
				u8g_DrawStr(&u8g, 0, 20,"Format dd/mm/yyyy");
				sprintf(buf,"%02d/%s/%4d",calendarDay,monthsInWords[month-1],year);

			}


  u8g_DrawStr(&u8g, 0, y_pos,buf);

}
