/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #8  Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Build a synchronous state machine that lights PB2PB1PB0 individual 
 *  for one second each 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/17nemI59nRka_o7op9J8DopskOKUEhVJ-/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include "timer.h"
#include "pwm.h"

enum STATES { OFF, ON, SCALE_UP, SCALE_DOWN, HOLD } State;
double frequency[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned char i = 0x00;
void SeeSaw_Speaker_Tick() {

    switch(State) { //Transitions
        case OFF:
            if((~PINA & 0x07) == 0x00) { State = OFF;}
            if((~PINA & 0x07) == 0x02) { State = OFF;}
            if((~PINA & 0x07) == 0x04) { State = OFF;}
            if((~PINA & 0x07) == 0x01) { State = ON;}
            break;
        case ON:
            if((~PINA & 0x07) == 0x00) { State = ON;}
            if((~PINA & 0x07) == 0x02) { State = SCALE_UP;}
            if((~PINA & 0x07) == 0x04) { State = SCALE_DOWN;}
            if((~PINA & 0x07) == 0x01) { State = OFF;}
            break;
        case SCALE_UP:
            if((~PINA & 0x07) == 0x00) { State = ON;}
            if((~PINA & 0x07) == 0x02) { State = HOLD;}
            if((~PINA & 0x07) == 0x04) { State = SCALE_DOWN;}
            if((~PINA & 0x07) == 0x01) { State = OFF;}
            break;
        case SCALE_DOWN:
            if((~PINA & 0x07) == 0x00) { State = ON;}
            if((~PINA & 0x07) == 0x02) { State = SCALE_UP;}
            if((~PINA & 0x07) == 0x04) { State = HOLD;}
            if((~PINA & 0x07) == 0x01) { State = OFF;}
            break;
        case HOLD:
            if((~PINA & 0x07) == 0x00) { State = ON;}
            if((~PINA & 0x07) == 0x02) { State = HOLD;}
            if((~PINA & 0x07) == 0x04) { State = HOLD;}
            if((~PINA & 0x07) == 0x01) { State = OFF;}
            break;
        default:
            printf("%s", "ERROR: Reached default state."); State = HOLD; break;
    }

    switch(State) { //State Actions
        case OFF:
            set_PWM(0);
            break;
        case ON:
            set_PWM(frequency[i]);
            break;
        case SCALE_UP:
            set_PWM(frequency[i]);
            if(i < 7) {i++;}
            break;
        case SCALE_DOWN:
            set_PWM(frequency[i]);
            if(i > 0) {i--;}
            break;
        case HOLD:
            set_PWM(frequency[i]);
            break;
        default:
            printf("%s", "ERROR: Reached default state action");
            break;
    }
}


int main (void) {

    DDRA = 0xF8; PORTA = 0xFF;
    DDRB = 0x40; PORTB = 0x00;

    State = OFF;
    PWM_on();

    while(1) {
        SeeSaw_Speaker_Tick();        
    }

    PWM_off();

    return 1;

}