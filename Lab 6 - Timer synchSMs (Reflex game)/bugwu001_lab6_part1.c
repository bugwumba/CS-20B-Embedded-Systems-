/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Build a synchronous state machine that lights PB2PB1PB0 individual 
 *  for one second each 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/10oqM0jC4SXT1j0TvMNfCDvXZbSmO_kgY/view?usp=sharing
 */

#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>

enum STATES {BLINK_PB0, BLINK_PB1, BLINK_PB2} State;
unsigned char tmpB = 0x00;

void SyncSM_LED_Tick() {

    switch(State) {//Transitions

        case BLINK_PB0:
            State = BLINK_PB1;
            break;
        case BLINK_PB1:
            State = BLINK_PB2;
            break;
        case BLINK_PB2:
            State = BLINK_PB0;
            break;
        default:
            printf("%s", "ERROR: Reached default state transition");
            State = BLINK_PB0;
	    break;
    }

    switch (State) {//Actions
        case BLINK_PB0:
            tmpB = 0x01;
            break;
        case BLINK_PB1:
            tmpB = 0x02; 
            break;
        case BLINK_PB2:
            tmpB = 0x04;
            break;
        default:
            printf("%s", "ERROR: Reached default state action");
	    break;
    }

    PORTB = tmpB;

}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0x07; //Set port B to output
    PORTB = 0x00; //Init port B to 0s
    TimerSet(1000); //The timer ticks every 1s
    TimerOn();
    State = BLINK_PB0;
    /* Insert your solution below */
    while (1) {
	//User code (i.e. synchSM calls)

	SyncSM_LED_Tick();
	while(!TimerFlag); //Wait 1 sec
	TimerFlag = 0;

	//Note: For the above a better style would use a synchSM with TickSM()
	//This example just illustrates the use of the ISR and flag

    }
    return 1;
}