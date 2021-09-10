/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 4 Exercise # 1
 *	Exercise Description: [optional - include for your own benefit] 

B0 is initially 1, B1 is zero. Pressing a button connected to PA0 
turns Bo = 0 and B1 = 1. Stays that way after button is released. 
WWhen button is pressed, B0 and B1 return to their initial states. 

 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
	Buddy Ugwumba
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>

enum STATES {Start, OFF_RELEASE, ON_PRESS, ON_RELEASE, OFF_PRESS} State;
unsigned char value = 0x00;

void AltLED_Tick(){

    switch(State){//Transitions
        case Start:
            State = OFF_RELEASE; value = 0x01; PORTB = value; break;
        case OFF_RELEASE:
            if(tmp!= 0x01){
                State = OFF_RELEASE;
            }
            else{
                State = ON_PRESS; value = 0x02; PORTB = value;
            }
            break;
        case ON_PRESS:
            if(tmp == 0x01){
                State = ON_PRESS;
            }
            else{
                State = ON_RELEASE;
            }
            break;
        case ON_RELEASE:
            if(tmp == 0x01){
                State = OFF_PRESS; value = 0x01; PORTB = value;
            }
            else{
                State = ON_RELEASE;
            }
            break;
        case OFF_PRESS:
            if(tmp == 0x01){
                State = OFF_PRESS;
            }
            else{
                State = OFF_RELEASE;
            }
            break;
        default:
            printf("%s", "Error: Reached default state.\n");
            State = OFF_RELEASE; break; 
    }
}

int main (void){

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    State = Start;

    while (1){
        AltLED_Tick();
    }

    return 1;
}