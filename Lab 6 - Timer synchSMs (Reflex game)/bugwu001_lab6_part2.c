/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Build a synchronous state machine that lights PB2PB1PB0 individual 
 *  for 1/3 of a second in each sequence then the currently lit LED stays lit 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/17TKnqipSEPw-Qwx12MihpIRs1Xla51A_/view?usp=sharing
 */

#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>

enum STATES {BLINK_PB0, BLINK_PB1, BLINK_PB2, WAIT, REVERSE} State;
unsigned char tmpB = 0x00;
unsigned char button;
unsigned char activateReverse = 0x00;

void SyncSM_LED_Tick() {

    button = ~PINA & 0xFF;

    switch(State) { //Transitions

        case WAIT:
            if(button == 0x00) { State = WAIT; }
            if(button == 0x01 && tmpB == 0x01) { State = BLINK_PB0; }
            if(button == 0x01 && tmpB == 0x02) { State = BLINK_PB1; }
            if(button == 0x01 && tmpB == 0x04) { State = BLINK_PB0; }
            break;
        case BLINK_PB0:
            if(button == 0x01) { State = WAIT; }
            if(button == 0x00) { State = BLINK_PB1; }
            break;
        case BLINK_PB1:
            if(button == 0x01) { State = WAIT; }
            if(button == 0x00 && activateReverse == 0x01) { State = BLINK_PB0;}
            if(button == 0x00 && activateReverse == 0x00) { State = BLINK_PB2; } 
            break;
        case BLINK_PB2:
            if(button == 0x01) { State = WAIT; }
            if(button == 0x00) { State = BLINK_PB1; } 
            break;
        default:
            printf("%s", "ERROR: Reached default state transition");
            State = BLINK_PB0;
	    break;
    }

    switch (State) { //Actions
        case WAIT:
             tmpB = tmpB;
             break;
        case BLINK_PB0:
            tmpB = 0x01;
            activateReverse = 0x00;
            break;
        case BLINK_PB1:
            tmpB = 0x02;  
            break;
        case BLINK_PB2:
            tmpB = 0x04;
            activateReverse = 0x01;
            break;
        default:
            printf("%s", "ERROR: Reached default state action");
	    break;
    }

    PORTB = tmpB;

}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFE; //Set port A to input
    PORTA = 0xFF; //Init port A to 1's
    DDRB = 0x07; //Set port B to output
    PORTB = 0x00; //Init port B to 0s

    TimerSet(300); //The timer ticks every 1s
    TimerOn();
    State = BLINK_PB0;

    /* Insert your solution below */
    while (1) {
	    //User code (i.e. synchSM calls)
        button = ~PINA & 0xFF;
	    SyncSM_LED_Tick();
	    while(!TimerFlag); //Wait 1 sec
	    TimerFlag = 0;
	    //Note: For the above a better style would use a synchSM with TickSM()
	    //This example just illustrates the use of the ISR and flag
    }
    return 1;
}