#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "font_8x8.h"
#include "oled.h"

//Address for the OLED commands
volatile char *OLED_cmd = (char *) 0x1000;
//Address for the OLED data
volatile char *OLED_data = (char *) 0x1200;

int OLED_init(void) {
	//Enable the external memory interface/4 bits address
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);

	//Setup the OLED display
	
	//display off
	*OLED_cmd = 0xAE;
	//segment remap
	*OLED_cmd = 0xA1;
	
	//common pads hardware: alternative
	*OLED_cmd = 0xDA; 
	*OLED_cmd = 0x12;
	
	//common output scan direction:com63~com0
	*OLED_cmd = 0xC8; 

	//multiplex ration mode:63
	*OLED_cmd = 0xA8; 
	*OLED_cmd = 0x3F;

	//display divide ratio/osc. freq. mode
	*OLED_cmd = 0xD5; 
	*OLED_cmd = 0x80;

	//contrast control
	*OLED_cmd = 0x81; 
	*OLED_cmd = 0x50;

	//set pre-charge period
	*OLED_cmd = 0xD9; 
	*OLED_cmd = 0x21;

	//set Memory Addressing Mode
	*OLED_cmd = 0x20; 
	*OLED_cmd = 0x02;

	//VCOM deselect level mode
	*OLED_cmd = 0xDB;
	*OLED_cmd = 0x30;

	//master configuration
	*OLED_cmd = 0xAD; 
	*OLED_cmd = 0x00;

	//out follows RAM content
	*OLED_cmd = 0xA4; 

	//set normal display
	*OLED_cmd = 0xA6;
	//display on
	*OLED_cmd = 0xAF; 
	
	//Set page start address
	*OLED_cmd = 0xB0;
	//Set lower column start address
	*OLED_cmd = 0x00;
	//Set higher column start address
	*OLED_cmd = 0x10;
	
	//Clear the display
	OLED_clear();
	OLED_home();

	return 0;
}

//What does it do?
int OLED_home(){
	*OLED_cmd = 0x21;
	*OLED_cmd = 0x00;
	*OLED_cmd = 0x7f;
	
	*OLED_cmd = 0x22;
	*OLED_cmd = 0x00;
	*OLED_cmd = 0x0f;
	
	return 0;
}

int OLED_goto_line(int line) {
	//Set lower column address
	*OLED_cmd = 0x00;
	//Set higher column address
	*OLED_cmd = 0x10;
	//Set page address
	if (line < 8) {
		*OLED_cmd = 0xB0;
		*OLED_cmd = 0xB0 | line;
	}

	return 0;
}

int OLED_pos(int row, int column) {
	OLED_home();
	OLED_goto_line(row);
	
	if (column < 16) {
		//Set lower column start address
		*OLED_cmd = 0x00 + (column*(FONTWIDTH)>>4);
		//Set higher column start address
		*OLED_cmd = 0x10 + (column*(FONTWIDTH)>>4);
	}
	
	return 0;
}

int OLED_clear_line(int line) {
	//Set page address
	OLED_goto_line(line);
	
	//Clear the page
	for(int i = 0; i < 128 ; i++) {
		*OLED_data=0x00;
	}	
	
	OLED_home();
	
	return 0;
}

int OLED_clear(){
	//Clear every page
	for (int k = 0; k < 8; k++) {
		OLED_clear_line(k);
	}
	
	return 0;
}

int OLED_print_char(char c) {
	//Write the complete character
	for(int i = 0; i < FONTWIDTH; i++) {
		*OLED_data = pgm_read_byte(&font[c-' '][i]);
	}
	return 0;
}

int OLED_print(char *data) {
	int i = 0;
	
	//Write the string(while (data[i] != 
	//while (data[i] > 0x1F && data[i] < 0x7F){
	while(data[i] != '\0'){
		OLED_print_char(data[i]);
		i++;
	}
	return 0;
}

/*
void OLED_start_scroll(){
	*OLED_cmd = 0x26;
	*OLED_cmd = 0x00;
	*OLED_cmd = 0x05;
	*OLED_cmd = 0x00;
	*OLED_cmd = 0x07;
	*OLED_cmd = 0x2F;
}*/