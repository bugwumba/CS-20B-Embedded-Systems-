/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Build a synchronous state machine that lights PB2PB1PB0 individual 
 *  for one second each 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1PxQlRL9YKkK3l_cHpX5a4wvqhxL9Iv8i/view?usp=sharing
 */

#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>

enum STATES {WAIT, INCREMENT, DECREMENT, RESET, HOLD} State;
unsigned char value = 0x07;
unsigned char temp;
unsigned char i = 0x00;
unsigned char j = 0x00;

//MUST COUNT EACH BUTTON PRESS ONCE
void ButtonCalc_Tick(){

    switch(State){//Transitions
   
        case WAIT:
            if(temp == 0x00 ) { State = WAIT; }
            if(temp == 0x01 && i <= 0x01) { State = INCREMENT; }
            if(temp == 0x02 && i <= 0x01) { State = DECREMENT; }
            if(temp == 0x03 && i <= 0x01) { State = RESET; }
            break;

        case INCREMENT:
            if(temp == 0x00){ State = WAIT; }
            if(temp == 0x01){ State = HOLD; }          
            if(temp == 0x02){ State = DECREMENT; }             
            if(temp == 0x03){ State = RESET; }
            break;

        case DECREMENT:
            if(temp == 0x00){ State = WAIT; }
            if(temp == 0x01){ State = INCREMENT; }            
            if(temp == 0x02){ State = HOLD; }
            if(temp == 0x03){ State = RESET;}
            break;

        case RESET:
            if(temp == 0x00) { State = WAIT; }
            if(temp == 0x01 || temp == 0x02 || temp == 0x03) { State = HOLD; }
            break;

        case HOLD:
            if(temp == 0x00){ State = WAIT; }
            if(temp == 0x01 && j != 0x0A) { State = HOLD; }
            if(temp == 0x02 && j != 0x0A) { State = HOLD; }  
            if(temp == 0x01 && j == 0x0A) { State = INCREMENT; }
            if(temp == 0x02 && j == 0x0A) { State = DECREMENT; }            
            if(temp == 0x03){ State = RESET;}
            break;

        default:
            printf("%s", "Error: Reached default state transition.\n"); State = WAIT; break; 
    }

    switch(State){//State actions
        case WAIT:
            if(i > 1) { i = 0; }
            i++;
            break;
        case INCREMENT:
            if(value < 0x09 ){ value++;} break;
        case DECREMENT:
            if(value > 0){ value--;} break;
        case RESET:
            value = 0x00; break;
        case HOLD:
            if(j > 0x0A) { j = 0; }
            j++;
            break;
        default:
            printf("%s", "Error: Reached default state action.\n"); break;    
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFC; //Set port A to input
    PORTA = 0xFF; //Init port A to 1's
    DDRB = 0x0F; //Set port B to output
    PORTB = 0x00; //Init port B to 0s

    TimerSet(100); //The timer ticks every 1s
    TimerOn();
    State = WAIT;
    PORTB = value;

    /* Insert your solution below */
    while (1) {
	    //User code (i.e. synchSM calls)
        temp = ~PINA & 0xFF;
	    ButtonCalc_Tick();
        PORTB = value;
	    while(!TimerFlag); //Wait 1 sec
	    TimerFlag = 0;

	    //Note: For the above a better style would use a synchSM with TickSM()
	    //This example just illustrates the use of the ISR and flag

    }
    return 1;
}