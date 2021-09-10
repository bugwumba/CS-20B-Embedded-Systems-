/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #14 Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *  Communication between two microcontrollers
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/14AC_gzoeyafJmrKQW5C78FSQZfmJmOVq/view?usp=sharing
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
#include "usart_ATmega1284.h"

enum Lead_Toggle {Start, Toggle, Transmit, Receive, Output, Decision};

static unsigned char local_variable = 0x00;
static unsigned char transmission_complete = 0x00;
static unsigned char received_variable = 0x00;
static unsigned char receive_complete = 0x00;
static unsigned char mode = 0x00;

int Follow_ToggleSMTick(int state){

    switch (state) { //State Transition
        case Start:
            state = Decision;
            break;
        case Decision:
            if((~PINB & 0x01) == 0x01) {state = Toggle;}
            if((~PINB & 0x01) == 0x00) {state = Receive;}
            break;
        case Toggle:
            state = Transmit;            
            break;
        case Transmit:
            if(transmission_complete == 0x00) {state = Transmit;}
            if(transmission_complete == 0x01) {state = Toggle;}
            if((~PINB & 0x01) == 0x00) {state = Receive; }
            break;
        case Receive:
            if(receive_complete == 0x00) {state = Receive;}
            if(receive_complete == 0x01) {state = Output;}
            if((~PINB & 0x01) == 0x01) {state = Toggle;}
            break;
        case Output:
            state = Receive;
            break;
        default:
            printf("%s", "ERROR: Reached default state transition");
            break;
    }

    switch (state) { //State Actions
        case Start:
            break;
        case Decision:
            break;
        case Toggle:
            mode = 0x01;
            transmission_complete = 0x00;
            local_variable = !local_variable;
            PORTA = local_variable;
            PORTC = mode; 
            break;
        case Transmit:
            if(USART_IsSendReady(1)){
                USART_Send(local_variable, 1);
            }
            if(USART_HasTransmitted(1)){
                transmission_complete = 0x01;
            }
            break;
        case Receive:
            mode = 0x00;
            if(USART_HasReceived(0)){
                received_variable = USART_Receive(0);
                receive_complete = 0x01;
            }
            USART_Flush(0); 
            USART_Flush(1);       
            break;
        case Output:
            PORTA = received_variable;
            PORTC = mode;
            receive_complete = 0x00;
            break;
        default:
            printf("%s", "ERROR: Reached default state transition");
            break;
    }

    return state;
}


int main(void) {

	DDRA = 0x01; PORTA = 0x00;
    DDRB = 0xFE; PORTB = 0xFF;
    DDRC = 0x01; PORTC = 0x00;
    DDRD = 0xF0; PORTD = 0xFF;

    //Declare an arry of tasks
    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //Task 1 DriveLED_SMTick
    task1.state = Start;//Task initial state.
    task1.period = 299;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time
    task1.TickFct = &Follow_ToggleSMTick;

    unsigned long GCD = tasks[0]->period;
    for(unsigned short i = 1; i < numTasks; i++){
        GCD = findGCD(GCD, tasks[i]->period);
    }

    //Set the timer and turn it on
    TimerSet(GCD);
    TimerOn();

    //Calling this function before the while loop initialize the USART for each microcontroller
    //I assume that that the USARTS for both microcontrollers should be initialized
    //So that both can transmit and recieve information from each other
    initUSART(0);
    initUSART(1);
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