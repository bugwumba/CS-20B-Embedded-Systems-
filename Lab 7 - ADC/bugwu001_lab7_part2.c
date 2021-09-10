/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #7  Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Build a synchronous state machine that lights PB2PB1PB0 individual 
 *  for one second each 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1CY_gRV-jJMXf4Q1277XzBCLgsYnvMF_d/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include <avr/interrupt.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	//Aden: setting this bit enables analog to digital conversion
	//ADSC: setting this bit astarts the first conversion
	//ADATE: setting this bit enables auto-triggering. Since we are
	//       in Free Running Mode, a new conversion will trigger whenever
	//       the previous conversion completes.

}

int main(void) {

	//input DDRA is set 0 for PA0; PORTA must be initialized to 1's
	DDRA = 0xFE; PORTA = 0xFF;
	//Outputs DDRB & DDRD are set to 1; PORTS B and D just be initialized to 0's
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0x3F; PORTD = 0x00;

	//Initialize analog to digital conversion
	ADC_init();
	unsigned short potentiometer = 0x00;
	unsigned char first_8 = 0x00;
	unsigned char last_2 = 0x00;

	while (1) {
		potentiometer = ADC;
		first_8 = (char)potentiometer;
		PORTB = first_8;
		last_2 = (char)(potentiometer >> 8);
		PORTD = last_2;
	}

    return 1;
}