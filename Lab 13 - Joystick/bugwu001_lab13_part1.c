/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #13 Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1ktwTEwX7GoRZxeLW4viNXtxT2J52ffbs/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include <stdlib.h>

void ADC_init() {
      ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}

// Pins on PORTA are used as input for A2D conversion
//    The default channel is 0 (PA0)
// The value of pinNum determines the pin on PORTA
//    used for A2D conversion
// Valid values range between 0 and 7, where the value
//    represents the desired pin for A2D conversion
void Set_A2D_Pin(unsigned char pinNum) {
    ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
    // Allow channel to stabilize
    static unsigned char i = 0;
    for ( i=0; i<15; i++ ) { asm("nop"); } 
}


int main(void) {
	//Outputs DDRB & DDRD are set to 1; PORTS B and D just be initialized to 0's
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	//Initialize analog to digital conversion
	unsigned short potentiometer = 0x0000;
	unsigned char first_8 = 0x00;
	unsigned char last_2 = 0x00;
	ADC_init();
	while (1) {
		potentiometer = ADC;
		first_8 = (char)potentiometer;
        PORTB = first_8;
		last_2 = (char)(potentiometer >> 8);       
		PORTD = last_2;
	}

    return 1;
}