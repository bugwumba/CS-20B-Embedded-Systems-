/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #13 Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1viG1CZ8gCdL64LhmdildxMhZVeCK-ZDB/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "scheduler.h"
#include "timer.h"

void ADC_init() {
      ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}

static unsigned char pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
static unsigned char row = 0xFE;  	    // Row(s) displaying pattern. 
							            // 0: display pattern on row
							            // 1: do NOT display pattern on row
	                                    // Transitions

static unsigned short potentiometer = 0x0000;


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

enum DriveLED_States {Display};
int DriveLED_SMTick(int state) {

	switch (state) { // State Transitions
		case Display:	
            break;
		default:	
            state = Display;
			break;
	}	
	                    
	switch (state) { //State Actions
        case Display:	
			break;
		default:
	        break;
	}
    
	transmit_data(pattern);	    // Pattern to display
	transmit_data2(row);		// Row(s) displaying pattern	
	return state;
}

enum ShiftLED_States {Start, Default, shiftRight, shiftLeft};
int ShiftLED_SMTick(int state) {

    potentiometer = ADC;

	switch (state) { // State Transitions
        case Start:
            state = Default;
            break;
		case Default:
            if(potentiometer > 519 && potentiometer < 1024) {state = shiftRight;}
            if(potentiometer < 519 && potentiometer >= 0) {state = shiftLeft;}
            if(potentiometer == 519) {state = Default;}
            break;
        case shiftRight:	
            if(potentiometer > 519 && potentiometer < 1024) {state = shiftRight;}
            if(potentiometer < 519 && potentiometer >= 0) {state = shiftLeft;}
            if(potentiometer == 519) {state = Default;}
            break;
        case shiftLeft:	
            if(potentiometer > 519 && potentiometer < 1024) {state = shiftRight;}
            if(potentiometer < 519 && potentiometer >= 0) {state = shiftLeft;}
            if(potentiometer == 519) {state = Default;}
            break;
		default:	
            state = Default;
			break;
	}	
	                    
	switch (state) { //State Actions
        case Start:
            break;
        case Default:
            break;
        case shiftRight:
            if(pattern == 0x01){
                pattern = 0x80;
            }
            else{
                pattern >>= 1;   
            }      
            break;
        case shiftLeft:
            if(pattern == 0x80){
                pattern = 0x01;
            }
            else{
                pattern <<= 1;
            }    
            break;
		default:
	        break;
	} 

	return state;
}

int main(void) {
	//Outputs DDRB & DDRD are set to 1; PORTS B and D just be initialized to 0's
	DDRC = 0x0F; PORTC = 0x00;
	DDRD = 0x0F; PORTD = 0x00;

	//Initialize analog to digital conversion
	ADC_init();

    //Declare an arry of tasks
    static task task1, task2;
    task *tasks[] = { &task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 DriveLED_SMTick
    task1.state = Display;//Task initial state.
    task1.period = 10;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time
    task1.TickFct = &DriveLED_SMTick;

    //Task 2 ShiftLED_SMTick
    task2.state = Start;//Task initial state.
    task2.period = 100;//Task Period
    task2.elapsedTime = task2.period;//Task current elapsed time
    task2.TickFct = &ShiftLED_SMTick;

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