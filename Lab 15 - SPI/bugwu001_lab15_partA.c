/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #15 Exercise # 0
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
#include "keypad.h"
#include "bit.h"
#include "scheduler.h"
#include "timer.h"

//Effects the columns of the LED Matrix
void transmit_data (unsigned char data){
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

//Affects the rows of the LED Matrix
void transmit_data2 (unsigned char data){
    int i;
    for(i = 8; i>=0; --i){
        //Sets SRCLR to 1 allowing data to be set
        //Also clears SRCLK in prepartion of sending data
        PORTA = 0x08;
        // set SER = nexct bit of data to be sent.
        PORTA |= ((data>>i) & 0x01);
        //set SRCLK =1. Rising edge shifts next bit of data into the shift register
        PORTA |= 0x02;
    }
    // set RCLK = 1. Rising edge copies data from "Shift" register to "Storage" register
    PORTA |= 0x04;
    // clears all lines in prepartion of a new transmission
    PORTA = 0x00;
}

static unsigned char x = 0x00;

enum KeyPad {Start, checkInput};

int KeyPad_SMTick(int state) {

    switch (state){ // State Transitions
        case Start:
            state = checkInput;
            break;
        case checkInput:
            state = checkInput;
            break;
        default:
            break;
    }

    switch (state) { //State Actions
        case Start:
            break;
        case checkInput:
            x = GetKeypadKey();
            switch(x) {
                case '\0': transmit_data(0x1F); break; // All 5 LEDS on
                case '1': transmit_data(0x01); break; // hex equivalent
                case '2': transmit_data(0x02); break; 
                case '3': transmit_data(0x03); break; 
                case '4': transmit_data(0x04); break; 
                case '5': transmit_data(0x05); break;
                case '6': transmit_data(0x06); break;
                case '7': transmit_data(0x07); break; 
                case '8': transmit_data(0x08); break;
                case '9': transmit_data(0x09); break;
                case 'A': transmit_data(0x0A); break; // hex equivalent
                case 'B': transmit_data(0x0B); break; 
                case 'C': transmit_data(0x0C); break; 
                case 'D': transmit_data(0x0D); break; 
                case '*': transmit_data(0x0E); break;
                case '0': transmit_data(0x00); break;
                case '#': transmit_data(0x0F); break; 
                default: transmit_data(0x1B); break; // Should nbever occur, Middle LED off. 
            }
            break;
        default:
            break;
    }

    return state;
}

int main (void){


    DDRA = 0x0F; PORTA = 0x00;
    //Keypad input
    DDRC = 0xF0; PORTC = 0x0F;
    //shift register input
    DDRD = 0xFF; PORTD = 0x00;

    //Declare an arry of tasks
    static task task1;
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 checks Keypad for new input
    task1.state = Start;//Task initial state.
    task1.period = 1000;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time
    task1.TickFct = &KeyPad_SMTick;

    unsigned long GCD = tasks[0]->period;
    for(unsigned short i = 1; i < numTasks; i++){
        GCD = findGCD(GCD, tasks[i]->period);
    }

    //Set the timer and turn it on
    TimerSet(GCD);
    TimerOn();
    
    while(1) {
        for (unsigned char i = 0; i < numTasks; i++){ //Scheudler code
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