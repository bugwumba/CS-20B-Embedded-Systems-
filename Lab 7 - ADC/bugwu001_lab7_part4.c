/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #7  Exercise # 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Build a synchronous state machine that lights PB2PB1PB0 individual 
 *  for one second each 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1kivk1HtOpV9Y4yjiJoyj9qdaxjEbjQ06/view?usp=sharing
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
     DDRB = 0xFF; PORTB = 0x00;

    //Initialize analog to digital conversion
	ADC_init();
	unsigned short potentiometer = 0x0000;
	unsigned char LED =  0x00;
	unsigned short maxValue = 165;
	unsigned char difference = 0x08;
	
    while (1) {
        potentiometer = ADC;
	
	    if(potentiometer >= maxValue) {
		    LED = 0xFF;
	    }
        else if(potentiometer >= (maxValue - difference)) {
		    LED = 0x7F;
	    }
        else if(potentiometer >= (maxValue - (difference *2))) {
		    LED = 0x3F;
	    }
        else if(potentiometer >= (maxValue - (difference *3))) {
		    LED = 0x1F;
	    }
        else if(potentiometer >= (maxValue - (difference * 4))) {
		    LED = 0x0F;
	    }
        else if(potentiometer >= (maxValue - (difference * 5))) {
		    LED = 0x07;
	    }
        else if(potentiometer >= (maxValue - (difference * 6))) {
		    LED = 0x03;
	    }
	    else if(potentiometer >= (maxValue - (difference * 7))) {
		    LED = 0x01;
	    }
	    else if(potentiometer <= (maxValue - (difference * 0))) {
		    LED = 0x00;
	    }
	
	    PORTB = LED;

    }

    return 1;
}