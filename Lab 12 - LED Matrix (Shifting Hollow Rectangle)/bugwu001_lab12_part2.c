/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #12 Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1Tor1NjIkmiei9sY9z6V2UR2mcsxtl0YP/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "timer.h"

//All LED's are displayed
static unsigned char pattern = 0x80;	// LED pattern - 0: LED off; 1: LED on
//0 displays patter on row
static unsigned char row = 0xFE;

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

enum Decide2 {Default_2, Cycle_Right, Cycle_Left, Decide_2, hold_2};

int Decide_2SMTick(int state){
    
    switch(state){ //State Actions
        case Default_2:
            if((~PINA & 0x00) == 0x00) { state = Default_2;}
            if((~PINA & 0x04) == 0x04) { state = Cycle_Right;}
            if((~PINA & 0x08) == 0x08) { state = Cycle_Left;}
            if((~PINA & 0x0C) == 0x0C) { state = Default_2;}
            break;
        case Cycle_Right:
            if((~PINA & 0x00) == 0x00) { state = Decide_2;}
            if((~PINA & 0x04) == 0x04) { state = hold_2;}
            if((~PINA & 0x08) == 0x08) { state = Cycle_Left;}
            if((~PINA & 0x0C) == 0x0C) { state = Default_2;}
            break;    
        case Cycle_Left:
            if((~PINA & 0x00) == 0x00) { state = Decide_2;}
            if((~PINA & 0x04) == 0x04) { state = Cycle_Right;}
            if((~PINA & 0x08) == 0x08) { state = hold_2;}
            if((~PINA & 0x0C) == 0x0C) { state = Default_2;}
            break;
        case Decide_2:
            if((~PINA & 0x00) == 0x00) { state = Decide_2;}
            if((~PINA & 0x04) == 0x04) { state = Cycle_Right;}
            if((~PINA & 0x08) == 0x08) { state = Cycle_Left;}
            if((~PINA & 0x0C) == 0x0C) { state = Default_2;}
            break;            
        case hold_2:
            if((~PINA & 0x00) == 0x00) { state = Decide_2;}
            if((~PINA & 0x04) == 0x04) { state = hold_2;}
            if((~PINA & 0x08) == 0x08) { state = hold_2;}
            if((~PINA & 0x0C) == 0x0C) { state = Default_2;}
            break;
        default:
            printf("%s", "ERROR: Reached default state transition");
            state = Default_2;
            break;
    }

    switch(state){ //State Actions
        case Default_2:
            pattern = 0x80;
            row = 0xE0;
            transmit_data(pattern);
            transmit_data2(row);
            break;
        case Cycle_Right:
            if(pattern != 0x01){
                pattern >>= 1;
            }
            break;    
        case Cycle_Left:
            if(pattern != 0x80){
                pattern <<= 1;                  
            }
            break;
        case Decide_2:
            transmit_data(pattern);
            transmit_data2(row);
            break;
        case hold_2:
            transmit_data(pattern);
            transmit_data2(row);
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
    static task task1;
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 2 Decide
    task1.state = Default_2;//Task initial state.
    task1.period = 10;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time
    task1.TickFct = &Decide_2SMTick;

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