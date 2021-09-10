/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #12 Exercise # 0
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "timer.h"

//Effects the columns of the LED Matrix
void transmit_data (unsigned char data){
    int i;
    for(i = 8; i>0; --i){
        //Sets SRCLR to 1 allowing data to be set
        //Also clears SRCLK in prepartion of sending data
        PORTC = 0x08;
        // set SER = nexct bit of data to be sent.
        PORTC |= ((data>>i) & 0x01);
        //set SRCLK =1. Rising edge shifts next bit of data into the shift register
        PORTC |= 0x02;
    }
    // set RCLK = 1. Rising edge copies data from "Shift" register to "Storage" register
    PORTC |= 0x04;
    // clears all lines in prepartion of a new transmission
    PORTC = 0x00;

    
}

//Affects the rows of the LED Matrix
void transmit_data2 (unsigned char data){
    int i;
    for(i = 8; i>=0; --i){
        //Sets SRCLR to 1 allowing data to be set
        //Also clears SRCLK in prepartion of sending data
        PORTD = 0x08;
        // set SER = nexct bit of data to be sent.
        PORTD |= ((data>>i) & 0x01);
        //set SRCLK =1. Rising edge shifts next bit of data into the shift register
        PORTD |= 0x02;
    }
    // set RCLK = 1. Rising edge copies data from "Shift" register to "Storage" register
    PORTD |= 0x04;
    // clears all lines in prepartion of a new transmission
    PORTD = 0x00;
}

//--------------------------------------
// LED Matrix Demo SynchSM
// Period: 100 ms
//--------------------------------------
enum Demo_States {shift};
int Demo_Tick(int state) {

	static unsigned char pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
	static unsigned char row = 0xFE;  	    // Row(s) displaying pattern. 
							                // 0: display pattern on row
							                // 1: do NOT display pattern on row
	                                        // Transitions

	switch (state) { // State Transitions
		case shift:	
            break;
		default:	
            state = shift;
			break;
	}	
	                    
	switch (state) { //State Actions
        case shift:	
            if (row == 0xEF && pattern == 0x01) { // Reset demo 
				    pattern = 0x80;		    
				    row = 0xFE;
			}
             else if (pattern == 0x01) { // Move LED to start of next row
				pattern = 0x80;
				row = (row << 1) | 0x01;
			} 
            else { // Shift LED one spot to the right on current row
				pattern >>= 1;
			}
			break;
		default:
	        break;
	}
    
	transmit_data(pattern);	    // Pattern to display
	transmit_data2(row);		// Row(s) displaying pattern	
	return state;
}


int main (void){

    DDRC = 0x0F; PORTC = 0x00;
    DDRD = 0x0F; PORTD = 0x00;

    //Declare an arry of tasks
    static task task1;
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 FestiveLights_1
    task1.state = shift;//Task initial state.
    task1.period = 100;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time
    task1.TickFct = &Demo_Tick;

    unsigned long GCD = tasks[0]->period;
    for(unsigned short i = 1; i < numTasks; i++){
        GCD = findGCD(GCD, tasks[i]->period);
    }

    //Set the timer and turn it on
    TimerSet(GCD);
    TimerOn();

    unsigned short i; //Scheduler for-loop iterator
    while(1) {
        for (i = 0; i < numTasks; i++){ //Scheudler code
            if(tasks[i]->elapsedTime == tasks[i]->period) { //Task is reasdy to tick
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0; //Reset the elapsed time for next tick

            }
            tasks[i]->elapsedTime += GCD;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 0; // Error: program should not exit
}