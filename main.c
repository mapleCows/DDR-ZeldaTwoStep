/*
 * DDR.c
 *
 * Created: 6/3/2019 11:16:46 AM
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
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
int UserScore = 0;
int MaxScore = 10;
int num = 10;
unsigned char buffer[5];
unsigned char printout[34] = "Max Score: ";
void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


/*
enum LCD_Score{wait1, display}Display;
	void Displays(){
		switch(Display){
			case -1:
				Display = wait1;
				break;
			case wait1:
				Display = display;
				break;
			case display:
				Display = wait1;
				break;
		}
		
		switch(Display){
			case -1: break;
			case wait1: 
				itoa(MaxScore, buffer, 10);
				//strcat(printout, buffer);
				//printout[17] = '\0';
				//printout[18] = '\0';
				LCD_ClearScreen();
				//LCD_DisplayString(1, printout);
				LCD_DisplayString(1, buffer);
				LCD_WriteCommand(0x0C);
				break;
			case display:break;
		}
};
	*/

int zeldaNotes[54] = {0,0,0,0,0,0,0,0,0,NOTE_D5,NOTE_F5,NOTE_D6,NOTE_D5,NOTE_F5,NOTE_D6,
					NOTE_E6,NOTE_F6,NOTE_E6,NOTE_F6,NOTE_E6,NOTE_C6,NOTE_A5,
					NOTE_A5,NOTE_D5,NOTE_F5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_D5,NOTE_F5,NOTE_G5,NOTE_E5,
					NOTE_D5,NOTE_F5,NOTE_D6,NOTE_D5,NOTE_F5,NOTE_D6,NOTE_E6,NOTE_F6,NOTE_E6,NOTE_F6,NOTE_E6,
					NOTE_C6,NOTE_A5,NOTE_A5,NOTE_D5,NOTE_F5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_D5};
					
int i = 0;
enum audio{wait_Music,zelda,zelda2} music;
	void sounds(){
			
			unsigned char ZeldaTones = PINA & 0x01;
			switch(music){
				case -1: music = wait_Music; break;
				case wait_Music:
					if(ZeldaTones){
						music = zelda;
					}
					else{music = wait_Music;}
					break;
				case zelda:
					if(!ZeldaTones){
						music = wait_Music;
					}
					else{
						music = zelda2;
					}
					break;
				case zelda2:
					if(!ZeldaTones){
						music = wait_Music;
					}
					else{
						music = zelda;
					}
				}
			switch(music){
				case wait_Music: set_PWM(0);
				break;
				case zelda:
					if(i > 52){
						i = 9;
					}
					set_PWM(zeldaNotes[i]);
					i++;
				break;
				case zelda2:
					if(i > 52){
						i = 9;
					}
					set_PWM(zeldaNotes[i]);
					i++;
					break;
			}
		
	};

enum ScoreState{left,up,down,right,wait,add,leftUp,leftDown,leftRight,UpDown,UpRight,DownRight} score;
	void scores(){
		unsigned char LeftFlag;
		unsigned char UpFlag;
		unsigned char DownFlag; 
		unsigned char RightFlag; 
		unsigned char A;
		int count = 0;
		switch(score){
			case -1:
			score = wait;
			
			case wait:
				A = PINA; 
				LeftFlag = A & 0x02;
				UpFlag = A & 0x04;
				DownFlag = A & 0x08;
				RightFlag = A & 0x10;
		/*if (LeftFlag){score = left;}
		
		else if (UpFlag){score = up;}
		
		else if (DownFlag){score = down;}
		
		else if (RightFlag){score = right;}
		
		else {score = wait;}
			*/
		
				/*if (LeftFlag){score = left;}
					
				else if (UpFlag){score = up;}
							
				else if (DownFlag){score = down;}
					
				else if (RightFlag){score = right;}
				else if (LeftFlag && DownFlag){score = leftDown;}	
				else if (LeftFlag + UpFlag == 0x06){score = leftUp;}
					
				else if (LeftFlag && RightFlag){score = leftRight;}
					
				else if (UpFlag + DownFlag == 0xA){score = UpDown;}
					
				else if (UpFlag && RightFlag){score = UpRight;}
					
				else if (DownFlag && RightFlag){score = DownRight;}
				else{score = wait;}
				*/
				score = wait;
				if (LeftFlag){score = left;}
				
				else if (UpFlag){score = up;}
				
				else if (DownFlag){score = down;}
				
				else if (RightFlag){score = right;}
				
				if (LeftFlag && DownFlag){score = leftDown;}
				else if (LeftFlag && UpFlag){score = leftUp;}
				
				else if (LeftFlag && RightFlag){score = leftRight;}
				
				else if (UpFlag && DownFlag){score = UpDown;}
				
				else if (UpFlag && RightFlag){score = UpRight;}
				
				else if (DownFlag && RightFlag){score = DownRight;}
			
				break;
				
			case left: 
				
				if(GetBit(snes_GetInput(),6)){
					score = add;
				}
				else{
					score = wait;
				}
				break;
			
		
			case up:
				if(GetBit(snes_GetInput(),4)){
					score = add;
				}
				else{
					score = wait;
				}
				break;
			case right: 
				if(GetBit(snes_GetInput(),7)){
					score = add;
				}
				else{
					score = wait;
				}
				break;
			case down:
			if(GetBit(snes_GetInput(),5)){
				score = add;
			}
			else{
				score = wait;
			}
			break; //leftUp,leftDown,leftRight,UpDown,UpRight,DownRight	//L = 6 R = 7 D = 5 U = 4
			
			case leftUp:
				if(GetBit(snes_GetInput(),6) || GetBit(snes_GetInput(),4)){	score = add;}
				else{score = wait;}
					//score = wait;
				break;
			
			case leftDown: 
				if(GetBit(snes_GetInput(),6) || GetBit(snes_GetInput(),5)){score = add;}
				else{score = wait;}
				break;
			case leftRight:
				if(GetBit(snes_GetInput(),6) || GetBit(snes_GetInput(),7)){score = add;}
				else{score = wait;}
				break;
			case UpDown:
				if(GetBit(snes_GetInput(),4) || GetBit(snes_GetInput(),5)){score = add;}
				else{score = wait;}
				break;
			case UpRight:
				if(GetBit(snes_GetInput(),4) || GetBit(snes_GetInput(),7)){score = add;}
				else{score = wait;}
				break;
			case DownRight:
				if(GetBit(snes_GetInput(),5) || GetBit(snes_GetInput(),7)){score = add;}
				else{score = wait;}
				break;
			
			
			
			case add:
			score = wait;
			break;
	}
			
		switch(score){
			case -1:
			UserScore = 0; break;
			case wait: count = 0; break;
			case left: 
			 break;
			case up:  
						
			break;
			
			case down: 
		
			break;
			case right: 
						
						break;
			case leftDown:
				//LCD_ClearScreen();
				//LCD_DisplayString(1,"leftDown");
			break;
			
			case leftUp:
			break;
			case add: 
				UserScore = UserScore + 1; 
				break;
		}
	};

enum MenuState{main_Menu,Song1,Finished_Song,reset} Menu;
	void Menus(){
		int temp;
		int dance;
		unsigned char EndSongFlag = PINA & 0x20;
		switch(Menu){
			EndSongFlag = PINA & 0x20;
			case -1:
			Menu = main_Menu;
			break;
			case main_Menu:
				if(GetBit(snes_GetInput(),3)){
					writeMax(0);
					MaxScore = 0;
				}
				
				if(GetBit(snes_GetInput(),2)){
					Menu = Song1;
				}
				else{
					Menu = main_Menu;
				}
				break;
			case Song1:
				if(EndSongFlag){
					Menu = Finished_Song;
				}
				else{
					Menu = Song1;
				}
				break;
			//case song2:
			//	if(EndSongFlag){
			//		Menu = Finished_Song;
			//	}
			//	else{
			//		Menu = song2;
			//	}
			//	break;
			case Finished_Song:
				Menu = main_Menu;
				break;
			case reset:
				Menu = main_Menu;
				break;
		}
		switch(Menu){
			case -1:
				break;
			case main_Menu:
				temp = readMax() + 1000;
				itoa(temp, buffer, 10);
				printout[30] = "Max Score ";
				printout[10] = buffer[1];
				printout[11] = buffer[2];
				printout[12] = buffer[3];
				printout[13] = buffer[4];
				printout[14] = '\0';
				printout[15] = '\0';
				LCD_ClearScreen();
				LCD_DisplayString(1, printout);
				LCD_WriteData(0x01);
				LCD_WriteData(0x00);
				LCD_WriteCommand(0x0C);
// 								LCD_Cursor(1);
// 			
// 								
// 								LCD_WriteData(0x00);
				PORTA = 0x00;
				UserScore = 0;
				break;
			case Song1:
				itoa(UserScore, buffer, 10);
				LCD_DisplayString(1,buffer);
				PORTA = 0x01;

				dance = UserScore % 4;
				if(dance == 0){
					LCD_WriteData(0x02);
				}
				if(dance == 1){LCD_WriteData(0x03);}
				if(dance == 2){LCD_WriteData(0x04);}
				if(dance == 3){LCD_WriteData(0x05);}
				
				break;
// 			case song2:
// 				itoa(UserScore, buffer, 10);
// 				LCD_DisplayString(1,buffer);
// 				PORTA = 0x40;
// 
// 				dance = UserScore % 4;
// 				if(dance == 0){
// 					LCD_WriteData(0x02);
// 				}
// 				if(dance == 1){LCD_WriteData(0x03);}
// 				if(dance == 2){LCD_WriteData(0x04);}
// 				if(dance == 3){LCD_WriteData(0x05);}
// 				
// 				break;
			case Finished_Song:
				if(UserScore > MaxScore){
					writeMax(UserScore);
					MaxScore = readMax();
				}
				PORTA = 0x00;
				break;
			case reset:
				writeMax(0);
				MaxScore = 0;
				break;
				
		}
};


int main(void){
	DDRA = 0x01; PORTA = 0xFE;
	DDRB = 0xFE; PORTB = 0x01;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	
	int temp = readMax();
	if(temp == 255){
		writeMax(0);
	}
	
	unsigned long int ScoreState_task_calc = 50;
	unsigned long int Menu_task_calc = 250;
	unsigned long int Music_task_calc = 500;

	
	
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(ScoreState_task_calc,Menu_task_calc);
	tmpGCD = findGCD(tmpGCD,Music_task_calc);
	unsigned long int GCD = tmpGCD;

	

	
	unsigned long ScoreState_task_period = ScoreState_task_calc/GCD;
	unsigned long Menu_task_period = Menu_task_calc/GCD;
	unsigned long Music_task_period = Music_task_calc/GCD;

	
	static task ScoreState_task, Menu_task,Music_task;
	task *tasks[] = { &ScoreState_task,&Menu_task,&Music_task};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	ScoreState_task.state = -1;
	ScoreState_task.period =  ScoreState_task_period;
	ScoreState_task.elapsedTime = ScoreState_task_period;
	ScoreState_task.TickFct = &scores;
	
	Menu_task.state = -1;
	Menu_task.period =  Menu_task_period;
	Menu_task.elapsedTime = Menu_task_period;
	Menu_task.TickFct = &Menus;
	
	Music_task.state = -1;
	Music_task.period =  Music_task_period;
	Music_task.elapsedTime = Music_task_period;
	Music_task.TickFct = &sounds;
	
	
	

	TimerSet(GCD);
	TimerOn();
	LCD_init();
	PWM_on();
	
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


