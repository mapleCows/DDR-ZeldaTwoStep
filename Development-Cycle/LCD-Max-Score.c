/*
 * LCDMaxScore.c
 *
 * Created: 6/3/2019 10:06:36 PM
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
unsigned char dance1[8] = {
	0x04,
	0x0E,
	0x15,
	0x0E,
	0x04,
	0x0A,
	0x00,
	0x00
};
const static unsigned char upA[8] = {0x04, 0x0A, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00};

const unsigned char player_pattern[8] = { 0x00, 0x0F, 0x1F, 0x10, 0x05, 0x10, 0x0F, 0x05 }; 


void LCD_WriteCustomData(unsigned char column, unsigned char *data_array) {
	int i;
	LCD_WriteCommand(0x40 + (column*8)); // Command 0x40 for GRAM
	for (i = 0; i < 8; i++) {
		LCD_WriteData(data_array[i]);
	}
	LCD_WriteCommand(0x80);
}

int main(void)
{
	DDRA = 0x01; PORTA = 0xFE;
	DDRB = 0xFE; PORTB = 0x01;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
    /* Replace with your application code */
   LCD_init();
   long long timer = 0;
   int  num =123;
   char max[15] = "Max Score";
   char buffer[5];
   char temp;
    while (1) 
    {
	if(timer > 800000){
		itoa(1125,buffer);
		max[14]='/0';
		max[10] = ':';
		max[9] = 'v';
		temp = buffer[0];
		max[11] = temp;
		max[12] = buffer[1];
		max[13] = buffer[2];
		//LCD_DisplayString(1,max);
		//LCD_CustomChar(1,dance1);
		//LCD_Custom_Char(1,dance1);
		//LCD_Custom_Char(5, player_pattern);	
		//LCD_Cursor(0);
		//LCD_LoadChar(0,dance1);
		//LCD_LoadChar(1, upA);
		//LCD_WriteCustomData(3,upA);
	// 		for (int i = 0; i < 8; i++) {
// 			LCD_WriteData(upA[i]);
// 		}
		LCD_DisplayString(1, "   Let's Play  ");
		LCD_WriteData(0x01);
		LCD_Cursor(1);
		LCD_WriteData(0x00);
// 		timer = 0;
//	LCD_WriteCommand(0x40); // Command 0x40 for GRAM
		}
		timer++;
    }
}

