#include <avr/io.h>
#include <util/delay.h>
#include "snes.h"
#include "bit.h"

#define LATCH  1 // Output
#define CLOCK  2 // Output
#define DATA   0 // Input

// SNES Controller Data Format: 0-16
/* Clock Cycle     Button Reported //https://gamefaqs.gamespot.com/snes/916396-super-nintendo/faqs/5395
 ===========     ===============
 1               B
 2               Y
 3               Select
 4               Start
 5               Up on joypad
 6               Down on joypad
 7               Left on joypad
 8               Right on joypad
 9               A
 10              X
 11              L
 12              R
 13              none (always high)
 14              none (always high)
 15              none (always high)
 16              none (always high)
*/

unsigned short snes_GetInput()
{
	unsigned short data = 0x0000;
	
	PORTB = SetBit(PINB, LATCH, 1);
	_delay_us(12);
	PORTB = SetBit(PINB, LATCH, 0);
	_delay_us(6);

	for (unsigned char i = 0; i < 12; i++)
	{
		if(!GetBit(PINB, DATA))
		{
			data |= (1<<i);
		}
		else
		{
			data &= ~(1<<i);
		}
		
		PORTB = SetBit(PINB, CLOCK, 1);
		_delay_us(6);
		PORTB = SetBit(PINB, CLOCK, 0);
		_delay_us(6);
	}

	return data;
}
