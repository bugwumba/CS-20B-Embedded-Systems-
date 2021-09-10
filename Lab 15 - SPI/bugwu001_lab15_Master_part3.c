/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #15 Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1ZIwxCo39H7gRKLkYwHkR0QtHdeh_ECBt/view?usp=sharing
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

static unsigned char j = 0x00;

// 8-bit value to be sent to the servant
//upper four bits hold the pattern number
//the lower four bits hold the speed number
static unsigned char data = 0x00;

static unsigned char Apattern[] = {0x60, 0x90, 0xF0, 0x90, 0x90};
static unsigned char Bpattern[] = {0xE0, 0x90, 0xE0, 0x90, 0xE0};
static unsigned char Cpattern[] = {0xF0, 0x80, 0x80, 0x80, 0xF0};
static unsigned char Dpattern[] = {0xE0, 0x90, 0x90, 0x90, 0xE0};
static unsigned char One_pattern[] = {0x02, 0x02, 0x02, 0x02, 0x02};
static unsigned char Two_pattern[] = {0x07, 0x01, 0x07, 0x04, 0x07};
static unsigned char Three_pattern[] = {0x07, 0x01, 0x07, 0x01, 0x07};
static unsigned char Four_pattern[] = {0x05, 0x05, 0x07, 0x01, 0x01};
static unsigned char Five_pattern[] = {0x07, 0x04, 0x07, 0x01, 0x07};
static unsigned char Six_pattern[] = {0x07, 0x04, 0x07, 0x05, 0x07};
static unsigned char display_rows[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF};
static unsigned char display_rows2[] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF};

static unsigned char decider1 = 0x00;
static unsigned char decider2 = 0x00;
static unsigned char display1 = 0x00;
static unsigned char display2 = 0x00;
static unsigned char display3 = 0x00;


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

enum DriveLED {Start1, Decide};

int DriveLED_SMTick (int state ){

    //Number
    decider1 = data << 4;
    //Letter
    decider2 = data >> 4;
    

    switch (state) { //State Transitions
        case Start1:
            state = Decide;
            break;
        case Decide:
            state = Decide;
            break;
        default:
            break;
    }

    switch (state) { //State Actions
        case Start1:
            break;
        case Decide:
            j++;
            if(j > 4){
                j = 0;
            }
            //check speed
            if(decider1 == 0x10){
                display1 = One_pattern[j];
            }
            if(decider1 == 0x20){
                display1 = Two_pattern[j];
            }
            if(decider1== 0x30){
                display1 = Three_pattern[j];
            }
            if(decider1 == 0x40){
                display1 = Four_pattern[j];
            }
            if(decider1 == 0x50){
                display1 = Five_pattern[j];
            }
            if(decider1== 0x60){
                display1 = Six_pattern[j];
            }    
            //check pattern
            if(decider2 == 0x01){
                display2 = Apattern[j];
            }
            if(decider2 == 0x02){
                display2 = Bpattern[j];
            }
            if(decider2 == 0x03){
                display2 = Cpattern[j];
            }
            if( decider2 == 0x04){
                display2 = Dpattern[j];
            }
            display3 = display2 | display1;
            break;
        default:
            break;
    }

    transmit_data(display3);
    transmit_data2(display_rows[j]);

    return state;

}

int main (void){

    //LED Matrix output
    DDRA = 0x0F; PORTA = 0x00;
    //PORTB is output on Master
    DDRB = 0xFF; PORTB = 0x00;
    //Keypad input
    DDRC = 0xF0; PORTC = 0x0F;
    //LED Matrix output
    DDRD = 0x0F; PORTD = 0x00;

    //Declare an arry of tasks
    static task task1, task2;
    task *tasks[] = { &task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 checks Keypad for new input
    task1.state = Start;//Task initial state.
    task1.period = 50;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time
    task1.TickFct = &KeyPad_SMTick;

    //Task 2 checks Keypad for new input
    task2.state = Start1;//Task initial state.
    task2.period = 1;//Task Period
    task2.elapsedTime = task2.period;//Task current elapsed time
    task2.TickFct = &DriveLED_SMTick;

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