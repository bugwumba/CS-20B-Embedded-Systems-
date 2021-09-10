/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #15 Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1nzKWjVegKR0LlWe82uKUs-_cS9TbhHqv/view?usp=sharing
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
#include "SPI.h"


// 8-bit value to be sent to the servant
//upper four bits hold the pattern number
//the lower four bits hold the speed number
static unsigned char data = 0x00;

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
                // it is best to make data  = data so that the last choice remains
                case '\0': data = data; break; 
                case '1': data = (data & 0x0F) | 0x40; break;  
                case '2': data = (data & 0x0F) | 0x30; break; 
                case '3': data = (data & 0x0F) | 0x20; break; 
                case '4': data = (data & 0x0F) | 0x40; break; 
                case '5': data = (data & 0xF0) | 0x06; break;
                case '6': data = (data & 0xF0) | 0x06; break;
                case '7': data = (data & 0xF0) | 0x06; break; 
                case '8': data = (data & 0xF0) | 0x06; break;
                case '9': data = (data & 0xF0) | 0x05; break;
                case 'A': data = (data & 0x0F) | 0x10; break; 
                case 'B': data = (data & 0xF0) | 0x03; break;
                case 'C': data = (data & 0xF0) | 0x02; break; 
                case 'D': data = (data & 0xF0) | 0x01; break; 
                case '*': data = (data & 0x0F) | 0x40; break; 
                case '0': data = (data & 0xF0) | 0x06; break; 
                case '#': data = (data & 0xF0) | 0x04; break; 
                default: data = 0x1B; break; // Should nbever occur, Middle LED off. 
            }
            SPI_MasterTransmit(data);
            break;
        default:
            break;
    }

    return state;
}

int main (void){

    //PORTB is output on Master
    DDRB = 0xFF; PORTB = 0x00;
    //Keypad input
    DDRC = 0xF0; PORTC = 0x0F;

    //Declare an arry of tasks
    static task task1;
    task *tasks[] = { &task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 checks Keypad for new input
    task1.state = Start;//Task initial state.
    task1.period = 50;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time
    task1.TickFct = &KeyPad_SMTick;

    unsigned long GCD = tasks[0]->period;
    for(unsigned short i = 1; i < numTasks; i++){
        GCD = findGCD(GCD, tasks[i]->period);
    }

    //Set the timer and turn it on
    TimerSet(GCD);
    TimerOn();
    SPI_MasterInit();
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