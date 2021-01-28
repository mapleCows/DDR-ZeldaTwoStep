/*
 * GameControlerLed.c
 *
 * Created: 5/23/2019 3:53:33 PM
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


#define LATCH  1 // Output
#define CLOCK  2 // Output
#define DATA   0 // Input
#define B  0
#define Y 1
#define Select 2
#define Start 3
#define Up 4
#define Down 5 
#define Left 6 
#define Right 7 
#define A 8
#define X 9
#define L 10
#define R 11


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

unsigned short snes_GetInput(){
	unsigned short data = 0x0000;
	
	PORTC = SetBit(PINC, LATCH, 1);
	_delay_us(12);
	PORTC = SetBit(PINC, LATCH, 0);
	_delay_us(6);

	for (unsigned char i = 0; i < 12; ++i){
		if(!GetBit(PINC, DATA)){
			data |= (1<<i);
		}
		else{
			data &= ~(1<<i);
		}
		
		PORTC = SetBit(PINC, CLOCK, 1);
		_delay_us(6);
		PORTC = SetBit(PINC, CLOCK, 0);
		_delay_us(6);
	}

	return data;
}


unsigned char LEDB = 0x00;
enum ledState{B_state,Y_state,Select_state,Start_state,Up_state,Down_state,Left_state,Right_state,A_state,X_state,L_state,R_state,selector} led;
void leds(){
	
	switch(led){
		case -1:
		led = B_state;
		break;
		
		case selector:
			if(GetBit(snes_GetInput(),0)){
				led = B_state;
			}
			else if(GetBit(snes_GetInput(),1)){
				led = Y_state;
			}
			else if(GetBit(snes_GetInput(),2)){
				led = Select_state;
			}
			else if(GetBit(snes_GetInput(),3)){
				led = Start_state;
			}
			else if(GetBit(snes_GetInput(),4)){
				led = Up_state;
			}
			else if(GetBit(snes_GetInput(),5)){
				led = Down_state;
			}
			else if(GetBit(snes_GetInput(),6)){
				led = Left_state;
			}
			else if(GetBit(snes_GetInput(),7)){
				led = Right_state;
			}
			else if(GetBit(snes_GetInput(),8)){
				led = A_state;
			}
			else if(GetBit(snes_GetInput(),9)){
				led = X_state;
			}
			else if(GetBit(snes_GetInput(),10)){
				led = L_state;
			}
			else if(GetBit(snes_GetInput(),11)){
				led = R_state;
			}
			
			else{
				led = selector;
			}
			break;

		case B_state: led =  selector; break;
		case Y_state: led = selector; break;
		case Select_state: led = selector; break;
		case Start_state: led = selector; break;
		case Up_state: led = selector; break;
		case Down_state: led = selector; break;
		case Left_state: led = selector; break;
		case Right_state: led = selector; break;
		case A_state: led = selector; break;
		case X_state: led = selector; break;
		case L_state: led = selector; break;
		case R_state: led = selector; break;
		
		


	}
			

	switch(led){
		case -1:
		PORTB = 0x00;
		break;
		case selector: PORTB = LEDB; break;
		case B_state: LEDB = 0x01; break;
		case Y_state: LEDB = 0x02; break;
		case Select_state: LEDB = 0X03; break;
		case Start_state: LEDB = 0X04; break;
		case Up_state: LEDB = 0X05; break;
		case Down_state: LEDB = 0X06; break;
		case Left_state: LEDB = 0X07; break;
		case Right_state: LEDB = 0X08; break;
		case A_state: LEDB = 0X10; break;
		case X_state: LEDB = 0X11; break;	
		case L_state: LEDB = 0X12; break;
		case R_state: LEDB = 0X14; break;
	}
};



int main(void){
	DDRA = 0xFF; PORTA = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	DDRC = 0xFE; PORTC = 0x01;
	DDRB = 0xFF; PORTB = 0x00;
	
	static task leds_task;
	task *tasks[] = { &leds_task};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	
	unsigned long leds_task_calc = 250;


	unsigned long int GCD = findGCD(leds_task_calc, leds_task_calc);
	

	
	unsigned long leds_task_period = leds_task_calc/GCD;

	 
	 leds_task.state = -1;
	  leds_task.period =  leds_task_period;
	  leds_task.elapsedTime = leds_task_period;
	  leds_task.TickFct = &leds;
	
	

	TimerSet(GCD);
	TimerOn();
	
	while(1) {
		// Task Scheduler
		for (unsigned short i = 0; i < numTasks; ++i ) {
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
}
