#include "helpers.h"
#include "lcd1602.h"

#define C1 8
#define C2 7
#define C3 6
#define C4 5

#define R1 12
#define R2 11
#define R3 10
#define R4 9

char _BUTTONS[] = {'7', '8', '9', '/', 
									'4', '5', '6', '*', 
									'1', '2', '3', '-', 
									'C', '0', '=', '+'};
int _COLUMNS[] =  { C4,  C3,  C2,  C1 };
int _ROWS[] =	 { R4,  R3,  R2,  R1 };

void delay_ms(int n){
	
	volatile int i;
	volatile int j;
	
	for( i = 0 ; i < n; i++) {
		for(j = 0; j < 6000; j++) {}
	}
}

void pins_initialize() {
	
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK; // enabling clock for both ports A and B
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; 
	
	for(int i = 0; i < 4; i++)
		PORTA->PCR[_COLUMNS[i]] |= PORT_PCR_MUX(1); //	setting MUX pins as a GPIO
	
	for(int i = 0; i < 4; i++)
		PORTA->PCR[_ROWS[i]] |= PORT_PCR_MUX(1);
	
	for(int i = 0; i < 4; i++)
		PTA->PDDR |= (1<<_ROWS[i]); // setting row pins as output
	
	for(int i = 0; i < 4; i++)
		PTA->PSOR |= (1<<_ROWS[i]); // setting high on row pins - turning off
	
	for(int i = 0; i < 4; i++) {
		PORTA->PCR[_COLUMNS[i]] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; // enabling pull-up resistors to improve reading of the keyboard
	}
}

void row_initialize(int ROW){
	
	PTA->PSOR |= (1<<R4); 
	PTA->PSOR |= (1<<R3);
	PTA->PSOR |= (1<<R2);
	PTA->PSOR |= (1<<R1); 
	
	if(ROW == R4 || ROW == R3 || ROW == R2 || ROW == R1)
		PTA->PCOR |= (1<<ROW); 		
	delay_ms(30); 
}

void LCD_initialize() {
	LCD1602_Init(); 
	LCD1602_Backlight(TRUE);
	LCD1602_SetCursor(0,0);
	LCD1602_Print("CALCULATOR");
	delay_ms(1000);
	LCD1602_ClearAll();
}

int count(char input[]) {

	int result = 0; 
	char first[16] = "";
	char second[16] = "";
	int operator_index = 0;
	int execute_index = 0;
	char operator = '0';

	for ( unsigned int i = 0; i < strlen(input) ; i++) {

    if ( input[i] == '=' ) { // checking if the operation is ready to be executed
        execute_index = i;
        break;
    }

		if ( input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' ){ // Checking input from keyboard to set mathematical operator
			operator_index = i;
			operator = input[i];
		}
	} 
	
	for(int i = 0; i < operator_index; i++ ) {
     strncat(&first[i], &input[i], 1); // appending characters from string
	}
	for(int i = 0; i < execute_index - operator_index; i++ ) {
     strncat(&second[i], &input[i + operator_index + 1], 1);
	}
	
	int first_number = atoi(first); //converting string to integer
  int second_number = atoi(second);
	
	if (operator == '+') 
		result = first_number + second_number;
	else if (operator == '-')
		result = first_number - second_number;
	else if (operator == '*')
		result = first_number * second_number;
	else if (operator == '/')
		result = first_number / second_number;

	return result; 
}
