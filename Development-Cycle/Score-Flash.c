/*
 * ScoringFlahs.c
 *
 * Created: 6/3/2019 1:39:05 PM
 * Author : Luis
 */ 

#include <avr/io.h>
#include "bit.h"
#include <stdio.h>
#include "io.h"
#include <util/delay.h>
#include "scheduler.h"
#include "timer.h"
#include "snes.h"
#include "eeprom.h"
#include "eeprom.c"
#include "snes.c"

int UserScore = -1;
int MaxScore = 0;


int main(void)
{
		DDRA = 0x01; PORTA = 0xFE;
		DDRB = 0xFE; PORTB = 0x01;
		DDRC = 0xFF; PORTC = 0x00; // LCD data lines
		DDRD = 0xFF; PORTD = 0x00; // LCD control lines
		int num = 10;
		unsigned char LeftFlag = PINA & 0x02;
		LCD_init();
		int UserScore = -1;
		int MaxScore = 0;
		
		unsigned char buffer[10];
		int timer = 0;
    while (1) 
    {
		LeftFlag = PINA & 0x02;
		if(LeftFlag){
			num = num + 1;
		}
		else{
			num = 10;
		}
						
						if(timer > 10000){
						itoa(num, buffer, 10);
						//strcat(printout, buffer);
						//printout[17] = '\0';
						//printout[18] = '\0';
						LCD_ClearScreen();
						//LCD_DisplayString(1, printout);
						LCD_DisplayString(1, buffer);
						LCD_WriteCommand(0x0C);
						timer = 0;
						}
						timer++;
		}
}

