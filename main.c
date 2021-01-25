#include "MKL05Z4.h" /*Device header*/
#include "lcd1602.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "helpers.h"

#define C1 8
#define C2 7
#define C3 6
#define C4 5

#define R1 12
#define R2 11
#define R3 10
#define R4 9

char BUTTONS[] = {'7', '8', '9', '/', 
									'4', '5', '6', '*', 
									'1', '2', '3', '-', 
									'C', '0', '=', '+'};
int COLUMNS[] =  { C4,  C3,  C2,  C1 };
int ROWS[] =	 { R4,  R3,  R2,  R1 };

int main (void) {
	
	pins_initialize();
	
	LCD_initialize();
	
	LCD1602_Print("COUNT:");
	//LCD1602_SetCursor(10,0);
	
	char button = '0'; 
	char result[16] = "";
	int reset = 0;
	char operator = '0';
	
	while(1){
			
			int press_check = 0;
		
			for(int c = 0; c < 4; c++){
					
				row_initialize(ROWS[c]);
				
				for(int r = 0; r < 4; r++){
					
					if( (strlen(result) == 0 ) && r == 3 ) // blocking operator as a first input
						continue;
					
					if( operator != '0' && r == 3) // disabling many operators
						continue;
					
					if ( reset == 1 ){
						if ( !(r == 0 && c == 3) )
							continue;
					}
					
					if( ( PTA->PDIR & (1<<COLUMNS[r]) ) == 0){		// checking if button is pressed				
						
						if(press_check){

							break;
						}
						else{
							button = BUTTONS[4 * c + r];
							press_check = 1;	

							if ( r == 3 )
								operator = button;		

							if( button == '=' ){
								
								if( strlen(result) == 0  || operator == '0' )
									continue;
								
								strncat(result, &button, 1);
								
								char output_number[16];
								int temp = count(result);
								sprintf(output_number, "%d", temp);
								
								LCD1602_SetCursor(0,1);
								LCD1602_Print("RESULT:");
								LCD1602_SetCursor(7,1);
								LCD1602_Print(output_number);
								
								reset = 1;
							}							
							
							if( button == 'C' ){
								
								if( strlen(result) == 0 ) {
									continue; 
								}
								
								char temp = result[strlen(result) - 1];
							  memset(result, 0, sizeof(result)); // setting memory to given value(result)
								operator = '0';
								
								if( temp == '=' ) {
									reset = 0;
								}
							
								LCD1602_ClearAll();
								LCD1602_Print("COUNT:");
							}
							else if (reset == 0) 
								strncat(result, &button, 1); // appending characters from string
					
						}
					}
				}
			}
			row_initialize(0);
			LCD1602_SetCursor(6,0);
			LCD1602_Print(result);
		}
}

