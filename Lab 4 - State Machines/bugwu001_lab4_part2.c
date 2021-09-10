/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 4 Exercise # 2
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

enum STATES {START, INCREMENT, DECREMENT, RESET, HOLD, WAIT} State;
unsigned char tmpC = 0x07;
void Latch(){
    switch(State){
        case START:
            State = WAIT; break;
        case WAIT:
            if(PINA == 0x00){ State = WAIT;}
            if(PINA == 0x01){ State = INCREMENT;}          
            if(PINA == 0x02){ State = DECREMENT; }             
            if(PINA == 0x03){ State = RESET; }
            break;
        case INCREMENT:
            if(PINA == 0x00){ State = WAIT; }
            if(PINA == 0x01){ State = HOLD; }            
            if(PINA == 0x02){ State = DECREMENT; }
            if(PINA == 0x03){ State = RESET;}
            break;
        case DECREMENT:
            if(PINA == 0x00){ State = WAIT; }
            if(PINA == 0x01){ State = INCREMENT; }            
            if(PINA == 0x02){ State = HOLD;}
            if(PINA == 0x03){ State = RESET; }
            break;
        case HOLD:
            if(PINA == 0x00){ State = WAIT; }
            if(PINA == 0x01){ State = HOLD; }
            if(PINA == 0x02){ State = DECREMENT; }
            if(PINA == 0x03){ State = RESET; }
            break;
        case RESET:
            if(PINA == 0x00){ State = WAIT; }
            if(PINA == 0x01){ State = RESET; }            
            if(PINA == 0x02){ State = RESET;}
            if(PINA == 0x03){ State = RESET; }
		break;
        default:
            State = WAIT; 
            break; 
    }
    switch(State){//State actions
        case START:
            break;
        case WAIT:
            break;
        case HOLD:
            break;
        case INCREMENT:
		if(tmpC <= 0x08)
		{
			tmpC++;
		}
	
        break;
        case DECREMENT:
		if(tmpC >= 0x01)
		{
			tmpC--;
		}
        break;
        case RESET:
            tmpC = 0x00;
            break;
        default:
            break;    
    }
PORTC = tmpC;
}

int main (void){
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    State = START;
    PORTC = tmpC;

    while (1){
        Latch();

    }
    return 1;
}