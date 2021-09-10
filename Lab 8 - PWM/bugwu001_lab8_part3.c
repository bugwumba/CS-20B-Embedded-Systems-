/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #8  Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Build a synchronous state machine that lights PB2PB1PB0 individual 
 *  for one second each 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  https://drive.google.com/file/d/1qBBzsZAy1FpRQZNBF0X4TiWcUHg8Indw/view?usp=sharing
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"
#include "pwm.h"

enum STATES { ON, PLAY_TUNE, PAUSE } State;
double frequency[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
volatile unsigned char j = 0x00;
volatile unsigned char i = 0x00;
unsigned char k = 0x00;
void Melody_Speaker_Tick() {

    switch(State) { //Transitions
        case ON:
            if((~PINA & 0x01) == 0x00) { State = ON;}
            if((~PINA & 0x01) == 0x01) { State = PLAY_TUNE;}
            break;
        case PLAY_TUNE:
            if((~PINA & 0x01) == 0x00) { 
                if(j == 1) {
                    State = PAUSE;
                }
                State = PLAY_TUNE;
            }
            if((~PINA & 0x01) == 0x01 ){
                if( i < 8 ) { 
                    State = PLAY_TUNE;
                }
                if( i == 8 ) { 
                    State = ON;
                }
	}
            if((~PINA & 0x01) == 0x00 && j == 1 ) { State = PAUSE;}
            break;
	case PAUSE:
		if((~PINA & 0x01) == 0x00) {
			if(i == 0){
				State = PLAY_TUNE;
			}
		 	State = PAUSE;
		}
	    if((~PINA & 0x01) == 0x01) { State = ON;}
	    break;
        default:
            printf("%s", "ERROR: Reached default state."); State = ON; break;
    }

    switch(State) { //State Actions
        case ON:
            set_PWM(0);
            break;
        case PLAY_TUNE:
            	set_PWM(frequency[rand() % 9]);
		if(i < 8) {
			i++;	
		}
            	if(i == 8){
                	j = 1;
            	}
		break;
	case PAUSE:
		set_PWM(0);
		if(i > 0){
			i = 0;
		}
		j = 0;
        default:
            printf("%s", "ERROR: Reached default state action");
            State = PLAY_TUNE;
            break;
    }
}


int main (void) {

    DDRA = 0xFE; PORTA = 0xFF;
    DDRB = 0x40; PORTB = 0x00;

    TimerSet(625); //The timer ticks every 1s
    TimerOn();

    srand(time(0));

    State = ON;
    PWM_on();

    while(1) {
        Melody_Speaker_Tick();
        while(!TimerFlag); //Wait 1 sec
	        TimerFlag = 0;        
    }

    PWM_off();

    return 1;

}