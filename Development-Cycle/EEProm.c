/*
 * EEprom.c
 *
 * Created: 6/1/2019 4:26:22 PM
 * Author : Luis
 */ 


#include <avr/eeprom.h>
#include <avr/io.h>
#include "bit.h"
#include <stdio.h>
#include "io.h"
#include "io.c"
#include <util/delay.h>
#include "scheduler.h"
#include "timer.h"

unsigned char readMax() {
	
	return eeprom_read_byte(0x0F);
}
void writeMax(unsigned char score) {
	eeprom_write_byte(0x0F, score);
}

int main(){
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFE; PORTB = 0x01;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	LCD_init();
	unsigned char B = PINB;
	int count = 0;
	unsigned long timer = 0x0000;
	while(1){
			count = readMax(count);
			if(timer >= 10000){
				unsigned char buffer[10] = {0};
				unsigned char printout[34] = "Max Score: ";
				itoa(count, buffer, 10);
				strcat(printout, buffer);
				printout[17] = '\0';
				printout[18] = '\0';
				LCD_ClearScreen();
				LCD_DisplayString(1, printout);
				timer = 0;
			}
			++timer;
}
	}