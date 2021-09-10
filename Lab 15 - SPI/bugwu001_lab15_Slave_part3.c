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
 * https://drive.google.com/file/d/1ZIwxCo39H7gRKLkYwHkR0QtHdeh_ECBt/view?usp=sharing
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

static unsigned char output1 = 0x00;
static unsigned char output2 = 0x00;
static unsigned char output3 = 0x00;
static unsigned char output4 = 0x00;
static unsigned char reverse_activate = 0x00;
static unsigned char bit_pattern1 = 0xF0;
static unsigned char bit_pattern2 = 0xAA;
static unsigned char bit_pattern3 = 0x80;
static unsigned char bit_pattern4 = 0xCC;
static unsigned char speed = 0x00;

enum pattern1 {Start1, pattern1};

int pattern1_SMTick (int state) {

    switch (state) { // State Transitions
        case Start1:
            state = pattern1;
            break;
        case pattern1:
            state = pattern1;
            break;
        default:
            break;
    }

    switch (state) { // State Actions
        case Start1:
            break;
        case pattern1:
            bit_pattern1 = ~bit_pattern1;
            output1 = bit_pattern1;
            break;
        default:
            break;
    }

    return state;
}

enum pattern2 {Start2, pattern2};

int pattern2_SMTick (int state) {

    switch (state) { // State Transitions
        case Start2:
            state = pattern2;
            break;
        case pattern2:
            state = pattern2;
            break;
        default:
            break;
    }

    switch (state) { // State Actions
        case Start2:
            break;
        case pattern2:
            bit_pattern2 = ~bit_pattern2;
            output2 = bit_pattern2;
            break;
        default:
            break;
    }

    return state;
}

enum pattern3 {Start3, pattern3};

int pattern3_SMTick (int state) {

    switch (state) { // State Transitions
        case Start3:
            state = pattern3;
            break;
        case pattern3:
            state = pattern3;
            break;
        default:
            break;
    }

    switch (state) { // State Actions
        case Start3:
            break;
        case pattern3:
            if(bit_pattern3 == 0x01){
                reverse_activate = 0x01;
            }        
            if(bit_pattern3 > 0x01 && reverse_activate == 0x00) {
                bit_pattern3 =  bit_pattern3 >> 1;
                output3 = bit_pattern3;                
            }
            if(bit_pattern3 < 0x80 && reverse_activate == 0x01){
                bit_pattern3 = bit_pattern3 << 1;
                output3 = bit_pattern3;
            }
            if(bit_pattern3 == 0x80){
                reverse_activate = 0x00;
            }
            break;
        default:
            break;
    }

    return state;
}

enum pattern4 {Start4, pattern4};

int pattern4_SMTick (int state) {

    switch (state) { // State Transitions
        case Start4:
            state = pattern4;
            break;
        case pattern4:
            state = pattern4;
            break;
        default:
            break;
    }

    switch (state) { // State Actions
        case Start4:
            break;
        case pattern4:
            bit_pattern4 = ~bit_pattern4;
            output4 = bit_pattern4;
            break;
        default:
            break;
    }

    return state;
}

enum output {Start5, output_SM};

int output_SMTick(int state) {

    switch (state) { // State Transitions
        case Start5:
            state = output_SM;
            break;
        case output_SM:
            state = output_SM;
            break;
        default:
            break;
    }

    switch (state) { // State Actions
        case Start5:
            break;
        case output_SM:
            if((receivedData & 0xF0) == 0x10){
                PORTC =  output1;            
            }
            if((receivedData & 0xF0) == 0x20){
                PORTC =  output2;            
            }       
            if((receivedData & 0xF0) == 0x30){
                PORTC =  output3;            
            } 
            if((receivedData & 0xF0) >= 0x40){
                PORTC =  output4;            
            }
            break;
        default:
            break;
    }

    return state;
}

enum DetermineSpeed {Start6, Read_Speed};

int DetermineSpeed_SMTick (int state){

    speed = receivedData & 0x0F;

    switch (state) { //State Transitions
        case Start6:
            state = Read_Speed;
            break;
        case Read_Speed:
            state = Read_Speed;
            break;
        default:
            break;
    }

    switch (state) { //State Actions
        case Start6:
            break;
        case Read_Speed:
            if(speed == 0x01){
                TimerSet(2000);
            }
            if(speed == 0x02){
                TimerSet(1000);
            }
            if(speed == 0x03){
                TimerSet(500);
            }
            if(speed == 0x04){
                TimerSet(250);
            }
            if(speed == 0x05){
                TimerSet(100);
            }
            if(speed == 0x06){
                TimerSet(50);
            }
            break;
        default:
            break;
    }

    return state;
}

int main (void){

    //PORTB is input in slave
    DDRB = 0x00; PORTB = 0xFF;
    //LED output
    DDRC = 0xFF; PORTC = 0x00;


    //Declare an arry of tasks
    static task task1, task2, task3, task4, task5, task6;
    task *tasks[] = { &task1, &task2, &task3, &task4, &task5, &task6};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task pattern1
    task1.state = Start1;//Task initial state.
    task1.period = 300;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time
    task1.TickFct = &pattern1_SMTick;
    
    //Task 2 pattern2
    task2.state = Start2;//Task initial state.
    task2.period = 300;//Task Period
    task2.elapsedTime = task2.period;//Task current elapsed time
    task2.TickFct = &pattern2_SMTick;

    //Task 3 pattern3
    task3.state = Start3;//Task initial state.
    task3.period = 300;//Task Period
    task3.elapsedTime = task3.period;//Task current elapsed time
    task3.TickFct = &pattern3_SMTick;

    //Task 4 pattern4
    task4.state = Start4;//Task initial state.
    task4.period = 300;//Task Period
    task4.elapsedTime = task4.period;//Task current elapsed time
    task4.TickFct = &pattern4_SMTick;

    //Task 5 output
    task5.state = Start5;//Task initial state.
    task5.period = 300;//Task Period
    task5.elapsedTime = task5.period;//Task current elapsed time
    task5.TickFct = &output_SMTick;

    //Task 6 output
    task6.state = Start6;//Task initial state.
    task6.period = 300;//Task Period
    task6.elapsedTime = task6.period;//Task current elapsed time
    task6.TickFct = &DetermineSpeed_SMTick;


    unsigned long GCD = tasks[0]->period;
    for(unsigned short i = 1; i < numTasks; i++){
        GCD = findGCD(GCD, tasks[i]->period);
    }

    //Set the timer and turn it on
    TimerSet(GCD);
    TimerOn();
    SPI_SlaveInit();
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