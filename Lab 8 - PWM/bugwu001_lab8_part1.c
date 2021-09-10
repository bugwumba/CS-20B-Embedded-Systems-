/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #8  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Build a synchronous state machine that lights PB2PB1PB0 individual 
 *  for one second each 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1zIsZ3iBQY3zNo-HGLUOwhp4mV8qC2iZJ/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include "timer.h"
#include "pwm.h"


enum STATES { WAIT, C4, D4, E4} State;

unsigned char tone;

void Speaker_Tick() {

    switch (State) { //Transitions
        
        case WAIT:
            if((~PINA & 0x07) == 0x00) { State = WAIT; }
            if((~PINA & 0x07) == 0x06) { State = WAIT; }
            if((~PINA & 0x07) == 0x03) { State = WAIT; }
            if((~PINA & 0x07) == 0x05) { State = WAIT; }
            if((~PINA & 0x07) == 0x07) { State = WAIT; }
            if((~PINA & 0x07) == 0x01) { State = C4; }
            if((~PINA & 0x07) == 0x02) { State = D4; }
            if((~PINA & 0x07) == 0x04) { State = E4; }
            break;
        case C4:
            if((~PINA & 0x07) == 0x00) { State = WAIT; }
            if((~PINA & 0x07) == 0x06) { State = WAIT; }
            if((~PINA & 0x07) == 0x03) { State = WAIT; }
            if((~PINA & 0x07) == 0x05) { State = WAIT; }
            if((~PINA & 0x07) == 0x07) { State = WAIT; }
            if((~PINA & 0x07) == 0x01) { State = C4; }
            if((~PINA & 0x07) == 0x02) { State = D4; }
            if((~PINA & 0x07) == 0x04) { State = E4; }
            break;
        case D4:
            if((~PINA & 0x07) == 0x00) { State = WAIT; }
            if((~PINA & 0x07) == 0x06) { State = WAIT; }
            if((~PINA & 0x07) == 0x03) { State = WAIT; }
            if((~PINA & 0x07) == 0x05) { State = WAIT; }
            if((~PINA & 0x07) == 0x07) { State = WAIT; }
            if((~PINA & 0x07) == 0x01) { State = C4; }
            if((~PINA & 0x07) == 0x02) { State = D4; }
            if((~PINA & 0x07) == 0x04) { State = E4; }
            break;
        case E4:
            if((~PINA & 0x07) == 0x00) { State = WAIT; }
            if((~PINA & 0x07) == 0x06) { State = WAIT; }
            if((~PINA & 0x07) == 0x03) { State = WAIT; }
            if((~PINA & 0x07) == 0x05) { State = WAIT; }
            if((~PINA & 0x07) == 0x07) { State = WAIT; }
            if((~PINA & 0x07) == 0x01) { State = C4; }
            if((~PINA & 0x07) == 0x02) { State = D4; }
            if((~PINA & 0x07) == 0x04) { State = E4; }
            break;
    }

    switch(State) {//Actions
        case WAIT:
            set_PWM(0);
            break;
        case C4:
            set_PWM(261.63);
            break;
        case D4:
            set_PWM(293.66);
            break;
        case E4:
            set_PWM(329.63);
            break;
    }
}

int main (void) {

    DDRA = 0xF8; PORTA = 0xFF;
    DDRB = 0x40; PORTB = 0x00;

    PWM_on();
    State = WAIT;

    while(1) {
        Speaker_Tick();
    }

    PWM_off();
    return 1;
}
