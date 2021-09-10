/*	Author: Buddy Ugwumba
 *  Partner(s) Name: None
 *	Lab Section: 021
 *	Assignment: Lab #9 Exercise # 4
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Buddy Ugwumba
 *  Demo Link: Youtube URL> 
 *  
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char combined = 0x00;

enum ThreeLEDSSM_STATES {PBO_ThreeLED, PB1_ThreeLED, PB2_ThreeLED} ThreeLED_State;
enum BlinkingLEDSM_State {PB3_BlinkingLED_ON, PB3_BlinkingLED_OFF} BlinkingLED_State;

void TickFct_ThreeLEDsSM() {
    switch (ThreeLED_State) { //State Transitions
        case PBO_ThreeLED:
            ThreeLED_State = PB1_ThreeLED;
            break;
        case PB1_ThreeLED:
            ThreeLED_State = PB2_ThreeLED;
            break;
        case PB2_ThreeLED:
            ThreeLED_State = PBO_ThreeLED;
            break;
        default:
            printf("%s", "ERROR: Reached default state transition.");
            ThreeLED_State = PBO_ThreeLED;
            break;
    }

    switch(ThreeLED_State) { //State Actions
        case PBO_ThreeLED:
            threeLEDs = 0x01;
            break;
        case PB1_ThreeLED:
            threeLEDs = 0x02;
            break;
        case PB2_ThreeLED:
            threeLEDs = 0x04;
            break;
        default:
            printf("%s", "ERROR: Reached default state action.");
            break;
    }

}

void TickFct_BlinkingLEDSM(){
    switch (BlinkingLED_State) { //State Transitions
        case PB3_BlinkingLED_ON:
            BlinkingLED_State = PB3_BlinkingLED_OFF;
            break;
        case PB3_BlinkingLED_OFF:
            BlinkingLED_State = PB3_BlinkingLED_ON;
            break;
        default:
            printf("%s", "ERROR: Reached default state transition.");
            BlinkingLED_State = PB3_BlinkingLED_ON;
            break;
    }

    switch(BlinkingLED_State) { //State Actions
        case PB3_BlinkingLED_ON:
            blinkingLED = 0x08;
            break;
        case PB3_BlinkingLED_OFF:
            blinkingLED = 0x00;
            break;
        default:
            printf("%s", "ERROR: Reached default state action.");
            break;
    }

}

enum PlaySpeakerSM {Speaker_Wait, Speaker_On, Speaker_Off} PlaySpeakerState;

unsigned char toggle = 0x00;
unsigned char j = 0x00;
unsigned char L = 0x02;
unsigned char H = 0x02;

void TickFct_PlaySpeakerSM (){
    switch (PlaySpeakerState){ //State Transitions
        case Speaker_On:
            if((~PINA & 0x04) == 0x04) {
                if(j <= H) {
                    PlaySpeakerState = Speaker_On;
                }
                PlaySpeakerState = Speaker_Off;
            }
            break;
        case Speaker_Off:
            if((~PINA & 0x04) == 0x00){ PlaySpeakerState = Speaker_Off; }
            if((~PINA & 0x04) == 0x04) {
                if(j <= L) {
                    PlaySpeakerState = Speaker_Off;
                }
                PlaySpeakerState = Speaker_On;
            }
            break;
        default:
            printf("'%s", "ERROR: Reached default State Transition");
            PlaySpeakerState = Speaker_Off;
            break;
    }
    
    switch(PlaySpeakerState){ //State Actions
        case Speaker_On:
            toggle = 0x10;
            if(j >= 1) {
                j = 0;
            }
            j++;
            break;
        case Speaker_Off:
            toggle = 0x00;
            if(j >= 1){
                j = 0;
            }
            j++;
            break;
        default:
            printf("%s", "ERROR: Reached default state actikon");
            toggle = 0x00;
            break;

    }

}

enum PollButtons {WAIT, SCALE_UP, SCALE_DOWN} PollButtons_state;

void TickFct_PollButtonsSM() {

    switch(PollButtons_state){ //State Transitions
        case WAIT:
            if((~PINA & 0x00) == 0x00) {PollButtons_state = WAIT; }
            if((~PINA & 0x01) == 0x01 && toggle == 0x01) {PollButtons_state = SCALE_UP; }
            if((~PINA & 0x02) == 0x02) {PollButtons_state = SCALE_DOWN; }
            break;
            // PA0 scales up
        case SCALE_UP:
            if((~PINA & 0x01) == 0x01) {PollButtons_state = SCALE_UP; }
            if((~PINA & 0x00) == 0x00) {PollButtons_state = WAIT; }
            if((~PINA & 0x01) == 0x02) {PollButtons_state = SCALE_DOWN; }
            break;
            // PA1 scales down
        case SCALE_DOWN:
            if((~PINA & 0x02) == 0x02) {PollButtons_state = SCALE_DOWN; }
            if((~PINA & 0x01) == 0x01) {PollButtons_state = SCALE_UP; }
            if((~PINA & 0x00) == 0x00) {PollButtons_state = WAIT; }
            break;
        default:
            printf( "%s", "ERROR: Reached default state transition");
            PollButtons_state = WAIT;
            break;

    }

    switch (PollButtons_state) { //State Actions
        case WAIT:
            toggle = toggle;
            break;
        case SCALE_UP:
            if(toggle < 8){
                toggle++;
            }
            break;
        case SCALE_DOWN:
            if(toggle > 1){
                toggle--;
            }
            break;
        default:
            printf("%s", "ERROR: Reached default state action");
            break;

    }


}


//ONLY ONE STATE MACHINE IS WRITING TO OUTPUTS
//DO THIS FOR THE REST OF THE QUARTER
void TickFct_CombineLEDsSM(){
    PORTB = threeLEDs + blinkingLED + toggle;;
}

int main (void) {
    unsigned long ThreeLED_elapsedTime = 0;
    unsigned long BlinkingLED_elapsedTime = 0;
    unsigned char PlaySpeaker_elapsedTime = 0;
    unsigned char PollButtons_elapsedTime = 0;
    unsigned char CombinedLEDSpeaker_elapsedTime = 0;
    const unsigned long timerPeriod = 2;
   //Set inputs to 0
    DDRA = 0xF8; PORTA = 0xFF;
    //Sets outputs to 1;    
    DDRB = 0x1F; PORTB = 0x00;
    TimerSet(timerPeriod);
    TimerOn();
    ThreeLED_State = PBO_ThreeLED;
    BlinkingLED_State = PB3_BlinkingLED_ON;
    PlaySpeakerState = Speaker_Off;
    PollButtons_state = WAIT;


    while(1){
        if(BlinkingLED_elapsedTime >= 1000){
            TickFct_BlinkingLEDSM();    
            BlinkingLED_elapsedTime = 0;        
        }
        if(ThreeLED_elapsedTime >= 300) {
            TickFct_ThreeLEDsSM();
            ThreeLED_elapsedTime = 0;            
        }
        if(PlaySpeaker_elapsedTime >= 2){
            TickFct_PlaySpeakerSM();
            PlaySpeaker_elapsedTime = 0;            
        }
        if(PollButtons_elapsedTime >= 2){
            TickFct_PollButtonsSM();
            PollButtons_elapsedTime = 0
        }
        if(CombinedLEDSpeaker_elapsedTime >= 2){
            TickFct_CombineLEDsSM(); 
            CombinedLEDSpeaker_elapsedTime = 0;           
        }

        while(!TimerFlag) {}
        TimerFlag = 0;
        BlinkingLED_elapsedTime += timerPeriod;
        ThreeLED_elapsedTime += timerPeriod;
        PlaySpeaker_elapsedTime += timerPeriod;
        PollButtons_elapsedTime += timerPeriod;
        CombinedLEDSpeaker_elapsedTime += timerPeriod;

    }

    return 1;
}