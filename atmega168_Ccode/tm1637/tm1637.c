
//  Author: avishorp@gmail.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <avr/delay.h>

#include "tm1637.h"
#define pinClk D,7
#define pinDIO B,0

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // B
  0b00111001,    // C
  0b01000111,    // D
  0b01111001,    // E
  0b01110001     // F
  };


void TM1637DisplayInit()
{
	// Set the pin direction and default value.
	// Both pins are set as inputs, allowing the pull-up resistors to pull them up
	__OUTPUT(pinClk);
	__OUTPUT(pinDIO);
	__LOW(pinClk);
	__LOW(pinDIO);
}

void setBrightness(uint8_t brightness)
{
	m_brightness = brightness;
}

void setSegments(const uint8_t segments[], uint8_t length, uint8_t pos)
{
    // Write COMM1
	start();
	writeByte(TM1637_I2C_COMM1);
	stop();

	cask();
	stop();
	// Write COMM2 + first digit address
	start();
	writeByte(TM1637_I2C_COMM2+(pos & 0x03));
	cask();
	// Write the data bytes
	for (uint8_t k=0; k < length; k++) {
	  writeByte(segments[k]);
		//writeByte(0xff);
		cask();
}
	stop();

	// Write COMM3 + brightness
	start();
	writeByte(0x8f);//TM1637_I2C_COMM3 + (m_brightness & 0x0f));
	cask();
	stop();
}

void showNumberDec(int num, bool leading_zero, uint8_t length, uint8_t pos)
{
	uint8_t digits[4];
	const static int divisors[] = { 1, 10, 100, 1000 };
	bool leading = true;

	for(int8_t k = 0; k < 4; k++) {
	    int divisor = divisors[4 - 1 - k];
		int d = num / divisor;

		if (d == 0) {
		  if (leading_zero || !leading || (k == 3))
		    digits[k] = encodeDigit(d);
	      else
		    digits[k] = 0;
		}
		else {
			digits[k] = encodeDigit(d);
			num -= d * divisor;
			leading = false;
		}
	}

	setSegments(digits + (4 - length), length, pos);
}

void bitDelay()
{
	_delay_us(50);
}

void start()
{
  __HIGH(pinClk);
  __HIGH(pinDIO);
  _delay_us(2);
  __LOW(pinDIO);


}

void stop()
{
	__LOW(pinClk);
	  _delay_us(2);
	__LOW(pinDIO);
	  _delay_us(2);
	__HIGH(pinClk);
	  _delay_us(2);
	__HIGH(pinDIO);

}

void writeByte(uint8_t b)
{
  uint8_t data = b;

  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // CLK low
	  __LOW(pinClk);

	// Set data bit
    if (data & 0x01)
    	__HIGH(pinDIO);
    else
    	__LOW(pinDIO);

    _delay_us(3);
    data = data >> 1;
	// CLK high
    __HIGH(pinClk);
    _delay_us(3);

  }

}

uint8_t encodeDigit(uint8_t digit)
{
	return digitToSegment[digit & 0x0f];
}

void clearDisplay(int start,int length) {

  uint8_t clearDisplay[] = { 0x00, 0x00, 0x00, 0x00 };

  setSegments(clearDisplay, length, start);
}
void cask() {
	__LOW(pinClk);
	_delay_us(5);
	__INPUT(pinDIO);
	//while(__READ(pinDIO));
	_delay_us(5);
	__OUTPUT(pinDIO);
	__HIGH(pinClk);
	_delay_us(2);
	__LOW(pinClk);
}