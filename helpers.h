
#include "frdm_bsp.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <cstring>


void delay_ms(int delay);	// setting up delay function 

void row_initialize(int ROW);	// initialization of row of the keyboard

int count(char input_string[]); // declaration of the whole logic of calculator

void LCD_initialize(); // initialization of LCD

void pins_initialize(); // initialization of pins
