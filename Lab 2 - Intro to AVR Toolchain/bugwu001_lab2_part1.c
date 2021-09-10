/*	Partner(s) Name & E-mail: None
 *	Lab Section: 22
 *	Assignment: Lab # 2 Exercise # 1
 *	Exercise Description: [optional - include for your own benefit] A garage door sensor connect to PA0 (1 means door is open), and a light sensor connects to PA1 (1 means light is sensed). Write a program that illuminates an LED connected to PB0 (1 means illuminate) if the garage door is open at night.
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
	Buddy Ugwumba
 */

#include <stdio.h>
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum STATES {START, ON, OFF} State;

void Tick_GarageLight() {
    
    //Write case Statements
    switch (State) { //Transitions

        case START: //Initial Transition
            State = OFF; break;

        case OFF:
            State = ((PINA == 0x00) | (PINA == 0x02) | (PINA == 0x03)) ? OFF : ON; break;

        case ON:
            State = (PINA == 0x01) ? ON : OFF; break;

        default:
            printf("%s", "ERROR: Reached deafult state Transition");
            break;
    }

    switch (State) { //State Actions
        case OFF:
            PORTB = 0x00;
            break;
        case ON:
            PORTB = 0x01;
            break;

        default:
            printf("%s", "ERROR: Reached default state action");
            break;
    }


}

int main(void) {

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xff; PORTB = 0x00;

    PORTB = 0x00;
    State = START;
    while(1) {
        Tick_GarageLight();
    }

    return 1;

}