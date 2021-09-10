/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 5 Exercise # 2
 *	Exercise Description: [optional - include for your own benefit] 

Buttons are connected to PA0 and PA1. Output for PORTC is initially 7.
Pressing PA0 increments PORTC once (stopping at nine). Pressing PA1
decrements PORTC once (stopping at 0). if Both Bottons are depressed
(even if not inititially simultaneously), PORTC resets to 0

 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
	Buddy Ugwumba
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>

enum STATES {WAIT, INCREMENT, DECREMENT, RESET, CLEAR} State;
unsigned char value = 0x07;
unsigned char temp;

//MUST COUNT EACH BUTTON PRESS ONCE
void ButtonCalc_Tick(){

    switch(State){//Transitions
   
        case WAIT:
            if(temp == 0x00) { State = WAIT; }
            if(temp == 0x01) { State = INCREMENT; }
            if(temp == 0x02) { State = DECREMENT; }
            if(temp == 0x03) { State = RESET; }
            break;

        case INCREMENT:
            if(temp == 0x00){ State = WAIT; }
            if(temp == 0x01){ State = CLEAR; }          
            if(temp == 0x02){ State = DECREMENT; }             
            if(temp == 0x03){ State = RESET; }
            break;

        case DECREMENT:
            if(temp == 0x00){ State = WAIT; }
            if(temp == 0x01){ State = INCREMENT; }            
            if(temp == 0x02){ State = CLEAR; }
            if(temp == 0x03){ State = RESET;}
            break;

        case RESET:
            if(temp == 0x00) { State = WAIT; }
            if(temp == 0x01) { State = CLEAR; }
            if(temp == 0x02) { State = CLEAR; } 
            if(temp == 0x03) { State = CLEAR; }
            break;

        case CLEAR:
            if(temp == 0x00){ State = WAIT; }
            if(temp == 0x01){ State = CLEAR; }            
            if(temp == 0x02){ State = CLEAR; }
            if(temp == 0x03){ State = RESET;}
            break;


        default:
            printf("%s", "Error: Reached default state transition.\n"); State = WAIT; break; 
    }

    switch(State){//State actions
        case WAIT:
            break;
        case INCREMENT:
            if(value < 0x09){ value++;} break;
        case DECREMENT:
            if(value > 0){ value--;} break;
        case RESET:
            value = 0x00; break;
        case CLEAR:
            break;
        default:
            printf("%s", "Error: Reached default state action.\n"); break;    
    }
}

int main (void){

    //input is zero
    //output is 1
    DDRA = 0xFC; PORTA = 0xFF;
    DDRC = 0x0F; PORTC = 0x00;

    State = WAIT;
    PORTC = value;

    while (1){
        temp = ~PINA & 0xFF;
        ButtonCalc_Tick();
        PORTC = value;
    }

    return 1;
}