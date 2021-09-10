/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #12 Exercise # 4
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1hwVJ3COSf_vs9O2nTg-GJKqA_lrpKarn/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "timer.h"

unsigned char pattern[] = {0x3C, 0x24, 0x3C};
unsigned char row[] = {0xFD, 0xFB, 0xF7};
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

enum Decide {Default, Cycle_Up, Cycle_Down, Cycle_Right, Cycle_Left, Decide, hold};

int DecideSMTick(int state){
    
    switch(state){ //State Actions
        case Default:
            if((~PINA & 0x00) == 0x00) { state = Default;}
            if((~PINA & 0x01) == 0x01) { state = Cycle_Up;}
            if((~PINA & 0x02) == 0x02) { state = Cycle_Down;}
            if((~PINA & 0x04) == 0x04) { state = Cycle_Right;}
            if((~PINA & 0x08) == 0x08) { state = Cycle_Left;}
            break;
        case Cycle_Up:
            if((~PINA & 0x00) == 0x00) { state = Default;}
            if((~PINA & 0x01) == 0x01) { state = hold;}
            if((~PINA & 0x02) == 0x02) { state = Cycle_Down;}
            if((~PINA & 0x04) == 0x04) { state = Cycle_Right;}
            if((~PINA & 0x08) == 0x08) { state = Cycle_Left;}
            break;    
        case Cycle_Down:
            if((~PINA & 0x00) == 0x00) { state = Default;}
            if((~PINA & 0x01) == 0x01) { state = Cycle_Up;}
            if((~PINA & 0x02) == 0x02) { state = hold;}
            if((~PINA & 0x04) == 0x04) { state = Cycle_Right;}
            if((~PINA & 0x08) == 0x08) { state = Cycle_Left;}
            break;
        case Cycle_Right:
            if((~PINA & 0x00) == 0x00) { state = Default;}
            if((~PINA & 0x01) == 0x01) { state = Cycle_Up;}
            if((~PINA & 0x02) == 0x02) { state = Cycle_Down;}
            if((~PINA & 0x04) == 0x04) { state = hold;}
            if((~PINA & 0x08) == 0x08) { state = Cycle_Left;}
            break;
        case Cycle_Left:
            if((~PINA & 0x00) == 0x00) { state = Default;}
            if((~PINA & 0x01) == 0x01) { state = Cycle_Up;}
            if((~PINA & 0x02) == 0x02) { state = Cycle_Down;}
            if((~PINA & 0x04) == 0x04) { state = Cycle_Right;}
            if((~PINA & 0x08) == 0x08) { state = hold;}
            break;                       
        case hold:
            if((~PINA & 0x00) == 0x00) { state = Default;}
            if((~PINA & 0x01) == 0x01) { state = hold;}
            if((~PINA & 0x02) == 0x02) { state = hold;}
            if((~PINA & 0x04) == 0x04) { state = hold;}
            if((~PINA & 0x08) == 0x08) { state = hold;}    
            break;
        default:
            printf("%s", "ERROR: Reached default state transition");
            state = Default;
            break;
    }

    switch(state){ //State Actions
        case Default:
            break;
        case Cycle_Up:
            if(row[0] != 0xFE){
                for(unsigned int k = 0; k < 3; k++){
                    row[k] = (row[k] >> 1) | 0x80;
                }
            }
            break;    
        case Cycle_Down:
            if(row[2] != 0xEF){
                for(unsigned int k = 0; k < 3; k++){
                    row[k] = (row[k] << 1) | 0x01;  
                }                
            }
            break;
        case Cycle_Right:
            if(pattern[0] != 0x0F){
                for(unsigned int k = 0; k < 3; k++){
                    pattern[k] >>= 1;  
                }                
            }
            break;
        case Cycle_Left:
            if(pattern[0] != 0xF0){
                pattern[0] <<= 1;  
                pattern[1] <<= 1;  
                pattern[2] <<= 1;  
            }
            break;
        case hold:
            break;            
        default:
            printf("%s", "ERROR: Reached default state transition");
            break;
    }

    return state;
}

int main (void){

    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0x0F; PORTC = 0x00;
    DDRD = 0x0F; PORTD = 0x00;

    //Declare an arry of tasks
    static task task1, task2;
    task *tasks[] = { &task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 Shift
    task1.state = Shift;//Task initial state.
    task1.period = 1;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time
    task1.TickFct = &Rectangle_Tick;

     //Task 2 Decide
    task2.state = Default;//Task initial state.
    task2.period = 10;//Task Period
    task2.elapsedTime = task2.period;//Task current elapsed time
    task2.TickFct = &DecideSMTick;

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