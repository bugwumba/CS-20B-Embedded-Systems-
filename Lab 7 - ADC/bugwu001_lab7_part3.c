/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #7  Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Build a synchronous state machine that lights PB2PB1PB0 individual 
 *  for one second each 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1Ebzuqor6UQeEbopsFE2SxaPP_s68Fb0v/view?usp=sharing
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
    //       the previous conversion completes./
}

int main(void) {

    //input DDRA is set 0 for PA0; PORTA must be initialized to 1's
     DDRA = 0xFE; PORTA = 0xFF;
     //Outputs DDRB & DDRD are set to 1; PORTS B and D just be initialized to 0's
     DDRB = 0x01; PORTB = 0x00;

    //Initialize analog to digital conversion
	ADC_init();
	unsigned short potentiometer = 0x0000;
	unsigned char LED =  0x00;
	unsigned short maxValue = 335;
	
    while (1) {
        potentiometer = ADC;
		if(potentiometer >= maxValue/2) {
			LED = (char)potentiometer;
		}
		if(potentiometer < maxValue/2){
			LED = 0x00;
		}
	
		PORTB = LED;

    }

    return 1;
}