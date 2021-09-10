/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #12 Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1QXDaw3jsMKdJFpT7ZOEmjgCqjF3CMowe/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "timer.h"


char pattern[] = {0x3C, 0x24, 0x3C};
char row[] = {0xFD, 0xFB, 0xF7};
unsigned int j = 0x00;

//Effects the columns of the LED Matrix
void transmit_data (unsigned char data){
    int i;
    for(i = 8; i>=0; --i){
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

enum Rectangle_States {Shift};

int Rectangle_Tick(int state) {

	switch (state) { // State Transitions
        case Shift:	
            state = Shift;
            break;
		default:	
            state = Shift;
			break;
	}	
	                    
	switch (state) { //State Actions
        case Shift:	
            if(j == 2){
                j = 0;
            }
            else if(j == 1){
                j++;
            }
            else{
                j++;
            }
            break;
		default:
	        break;
	}

    transmit_data(pattern[j]);
    transmit_data2(row[j]);
    	
	return state;
}

int main (void){

    DDRC = 0x0F; PORTC = 0x00;
    DDRD = 0x0F; PORTD = 0x00;

    //Declare an arry of tasks
    static task task1;
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 2 Decide
    task1.state = Shift;//Task initial state.
    task1.period = 1;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time
    task1.TickFct = &Rectangle_Tick;

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