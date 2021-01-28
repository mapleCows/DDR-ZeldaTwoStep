#include <avr/eeprom.h>
#include <avr/io.h>
#include "bit.h"
#include <stdio.h>
#include "io.h"
#include "io.c"
#include <util/delay.h>
#include "scheduler.h"
#include "timer.h"
#include "eeprom.h"

unsigned char readMax() {
	
	return eeprom_read_byte(0x0F);
}
void writeMax(unsigned char score) {
	eeprom_write_byte(0x0F, score);
}
